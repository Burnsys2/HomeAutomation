WiFiClient ethClient;
PubSubClient mqttClient(ethClient);
//SimpleTimer tReconnect;
long NetpreviousMillis = 0;
long Netinterval = 10000;

void setupEthernet()
{
	Serial.println(F("Configurando Red"));
	mqttClient.setServer(mqtt_server, 1883);
	mqttClient.setCallback(callback);
	//EthernetClient client = server.available();
	Serial.println(F("Fin Configurar red"));
	Reconnect();
}
bool ProcesarRed()
{
	ArduinoOTA.handle();
	unsigned long currentMillis = millis();
   if(currentMillis - NetpreviousMillis > Netinterval) {
		NetpreviousMillis = currentMillis;   
		Reconnect();
 	 }

	if (!mqttClient.connected()) return false;
	mqttClient.loop();
	return mqttClient.connected();
}
void Reconnect()
{
	//disable watchdog
	if (mqttClient.connected()) return;
	char buffers[25];
	sector.toCharArray(buffers, 25);

	WiFi.begin(ssid, password);
	WiFi.hostname(buffers);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}

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

void sendMqttfPrec(String topic, float value, bool retained)
{

	if (!mqttClient.connected()) return;
	char buffert[50];
    char buffer[10];
    dtostrf(value, 0, 2, buffer);
	String topicFinal = globalTopic + "/" + sector + "/out/" + topic;	
	topicFinal.toCharArray(buffert,50);
    mqttClient.publish(buffert, buffer,retained);
}
void reportIp()
{
	IPAddress ip = WiFi.localIP();

	sendMqttf("IP", String(ip[0]) + String(".") + String(ip[1]) + String(".") + String(ip[2]) + String(".") + String(ip[3]), true);
}

void reportRSSI()
{
	long rssi = WiFi.RSSI();
	sendMqttf("RSSI", rssi, false);
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
	if (topico == F("WSSTRIP"))
		ProcesarComandoWSLedsStrip(topic,valor);
	if (topico == F("RESET"))
		resetFunc();  //call reset
	if (topico == F("SOFTRESET"))
		ESP.reset();
	if (topico == F("RELAY"))
		ProcesarComandoRelays(topic,valor);
	if (topico == F("ENCODER"))
		ProcesarComandoEncloders(topic, valor);
	if (topico == F("WDT"))
		ProcesarComandoWDT(topic,valor);
	if (topico == F("PING"))
		sendMqttf(F("PONG") ,1 ,false);
	if (topico == F("SENSORES"))
		ProcesarComandoSensores(topic,valor);
}
