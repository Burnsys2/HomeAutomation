// Incluimos librer�a
// Enter a MAC address and IP address for your controller below.
EthernetClient ethClient;
PubSubClient mqttClient(ethClient);
SimpleTimer tReconnect;
void setupEthernet()
{
	Serial.println(F("Configurando Red"));
	Ethernet.begin(mac, ip);
	Serial.println(F("Configurando Callback Mqtt"));
	
    mqttClient.setServer(mqtt_server, 1883);
	mqttClient.setCallback(callback);
	//EthernetClient client = server.available();
	Reconnect();
	tReconnect.setInterval(10000,Reconnect);
	Serial.println(F("Fin Configurar red"));
}
bool ProcesarRed()
{
	tReconnect.run();
	if (!mqttClient.connected()) return false;
 	mqttClient.loop();
	return mqttClient.connected();
}
void Reconnect()
{
	//disable watchdog
	if (mqttClient.connected()) return;
	
	if (Ethernet.hardwareStatus() == EthernetNoHardware) {
		Serial.println(F("Ethernet shield was not found.  Sorry, can't run without hardware. :("));
		return;
	}
	if (Ethernet.linkStatus() == LinkOFF) {
		Serial.println(F("Ethernet cable is not connected."));
		return;
	}

	switch (Ethernet.maintain()) {
		case 1:
			//renewed fail
			Serial.println(F("Error: renewed fail"));
			return;
		case 2:
			//renewed success
			Serial.println(F("Renewed success"));
			//print your local IP address:
			Serial.print(F("My IP address: "));
			Serial.println(Ethernet.localIP());
			break;

		case 3:
			//rebind fail
			Serial.println(F("Error: rebind fail"));
			return;

		case 4:
			//rebind success
			Serial.println(F("Rebind success"));
			//print your local IP address:
			Serial.print(F("My IP address: "));
			Serial.println(Ethernet.localIP());
			break;

		default:
			//nothing happened
			break;
	}

	Serial.print(millis());
	Serial.print(F(" - mqtt Conecting: "));
	Serial.println(mqtt_server);
	String Topicrelays = globalTopic + "/" + sector + "/in/#";
	char buffert[50];
	Topicrelays.toCharArray(buffert,50);
	mqttClient.connect(buffert);
	mqttClient.subscribe(buffert);
	Serial.print(millis());
	Serial.println(F(" - fin Reconnect"));
	Serial.println(mqttClient.connected());
	//enable watchdog
	reportIp();
}

void sendMqttf(String topic, String value, bool retained)
{
	if (!mqttClient.connected()) return;
    char buffert[50];
    char buffer[50];

	value.toCharArray(buffer,50);
	String topicFinal = globalTopic + "/" + sector + "/out/" + topic;	
	topicFinal.toCharArray(buffert,50);
    mqttClient.publish(buffert, buffer,retained);
}

//void sendMqttfAsBool(String topic, float value, bool retained)
//{
//	String valueTmp = "false";
//	if (value == 1)	{valueTmp = "true";	}
//	sendMqttf(topic,valueTmp,retained);
//}

void sendMqttf(String topic, float value, bool retained)
{
  //  Serial.println("aa");

	if (!mqttClient.connected()) return;
  //  Serial.println("OK");
	char buffert[50];
    char buffer[10];
    dtostrf(value, 0, 0, buffer);
	String topicFinal = globalTopic + "/" + sector + "/out/" + topic;	
	topicFinal.toCharArray(buffert,50);
    mqttClient.publish(buffert, buffer,retained);
}
void reportIp()
{
	sendMqttf("IP",IpAddress2String(Ethernet.localIP()),true);
}

void callback(char* topic, byte* payload, unsigned int length) {
    
   /* Serial.print(millis());
    Serial.print(" - Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    Serial.println("");
    */
 //  	setLedAction(ReceiveAction);
	BlinkLedStatus = ReceiveAction;
 	String topico = getValue(topic,'/',3);
	topico.toUpperCase();
 	if (topico == F("IR"))
	{
		ProcesarComandoIR(topic,payload, length);
		return;
	}

	String valor = array_to_string(payload,length);
	valor.toUpperCase();
//	delete[] payload;

	if (topico == F("RELAY"))
	{
		ProcesarComandoRelays(topic,valor);
	}
	if (topico == F("PING"))
	{
		sendMqttf(F("PONG") ,1 ,false);
	}
	if (topico == F("LEDSRGB"))
	{
		ProcesarComandoLedsRgb(topic,valor);
	}
		if (topico == F("WSSTRIP"))
	{
		ProcesarComandoWSLedsStrip(topic,valor);
	}
    if (topico == F("RF433") || topico == F("RF315"))
	{
		ProcesarComandoRF(topic,valor);
	}
	
	if (topico == F("RESET"))
	{
		resetFunc();  //call reset	
	}
	if (topico == F("SOFTRESET"))
	{
		softReset();
	}
		if (topico == F("SENSORES"))
	{
		ProcesarComandoSensores(topic,valor);
	}

}
