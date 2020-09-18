WiFiClient ethClient;
PubSubClient mqttClient(ethClient);
SimpleTimer tReconnect;

void setupEthernet()
{
	Serial.println(F("Configurando Red"));
	mqttClient.setServer(mqtt_server, 1883);
	mqttClient.setCallback(callback);
	//EthernetClient client = server.available();
    tReconnect.setInterval(10000, Reconnect);
	Serial.println(F("Fin Configurar red"));
	Reconnect();
}
bool ProcesarRed()
{
	tReconnect.run();	
	if (!mqttClient.connected()) return false;
	mqttClient.loop();
	ArduinoOTA.handle();

	return mqttClient.connected();
}
void Reconnect()
{
	//disable watchdog
	if (mqttClient.connected()) return;
	WiFi.begin(ssid, password);

	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

	char buffers[50];
	sector.toCharArray(buffers, 50);

	ArduinoOTA.setHostname(buffers);
	ArduinoOTA.begin();

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());
	Serial.print(F(" - mqtt Conecting: "));
	Serial.println(mqtt_server);
	String Topicrelays = globalTopic + "/" + sector + "/in/#";
	char buffert[50];
	Topicrelays.toCharArray(buffert, 50);
	mqttClient.connect(buffert);
	mqttClient.subscribe(buffert);
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

	value.toCharArray(buffer, 50);
	String topicFinal = globalTopic + "/" + sector + "/out/" + topic;
	topicFinal.toCharArray(buffert, 50);
	mqttClient.publish(buffert, buffer, retained);
}
void reportIp()
{
	IPAddress ip = WiFi.localIP();

	sendMqttf("IP", String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]), true);
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
	topicFinal.toCharArray(buffert, 50);
	mqttClient.publish(buffert, buffer, retained);
}

void callback(char* topic, byte* payload, unsigned int length) {
/*
	 Serial.print(millis());
	 Serial.print(" - Message arrived [");
	 Serial.print(topic);
	 Serial.print("] ");
	 Serial.println("");
*/	 
	BlinkLedStatus = ReceiveAction;
	String topico = getValue(topic, '/', 3);
	topico.toUpperCase();
	
	if (topico == F("IR"))
	{
		ProcesarComandoIR(topic,payload, length);
		return;
	}


	String valor = array_to_string(payload,length);
	valor.toUpperCase();
//	delete[] payload;
	if (topico == F("WSSTRIP"))
	{
		ProcesarComandoWSLedsStrip(topic,valor);
	}
	if (topico == F("RESET"))
	{
		resetFunc();  //call reset
	}
	if (topico == F("SOFTRESET"))
	{
		ESP.reset();
	}
	
	if (topico == F("RELAY"))
	{
		ProcesarComandoRelays(topic,valor);
	}
	/*
	if (topico == F("LEDSRGB"))
	{
		ProcesarComandoLedsRgb(topic,valor);
	}
	if (topico == F("RF433") || topico == F("RF315"))
	{
		ProcesarComandoRF(topic,valor);
	}

*/
	if (topico == F("PING"))
	{
		sendMqttf(F("PONG") ,1 ,false);
	}
	if (topico == F("SENSORES"))
	{
		ProcesarComandoSensores(topic,valor);
	}
}
