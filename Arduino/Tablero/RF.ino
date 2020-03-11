RCSwitch mySwitch433;
void setupRF()
{
	if (rf433InPin >= 1 || rf433OutPin >= 1)
	{
		mySwitch433 = RCSwitch();
		mySwitch433.setProtocol(1);
		mySwitch433.setRepeatTransmit(15);
		if (rf433InPin >= 1) {
			mySwitch433.enableReceive(digitalPinToInterrupt(rf433InPin));
			Serial.print(F("R RF433 ON "));
			Serial.println(digitalPinToInterrupt(rf433InPin));
		}
		if (rf433OutPin >= 1) {
			mySwitch433.enableTransmit(rf433OutPin);
			pinMode(rf433OutPin, OUTPUT);
		}
	}
}

void ProcesarRF()
{
	if (rf433InPin >= 1 && mySwitch433.available())
	{
		sendMqttf("Sensores/RF433", mySwitch433.getReceivedValue(), false);
		mySwitch433.resetAvailable();
		BlinkLedStatus = RFDetected;
	}
}

void ProcesarComandoRF(String topic, String valor)
{
	Serial.println(topic);
	String tipo = getValue(topic, '/', 3);
	int nroPin = getValue(topic, '/', 4).toInt();
	char buffert[25];
	valor.toCharArray(buffert, 25);
	if (nroPin > 0)
		mySwitch433.enableTransmit(nroPin);
	else
		mySwitch433.enableTransmit(rf433OutPin);
	mySwitch433.send(buffert);
	
}