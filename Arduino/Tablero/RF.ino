RCSwitch mySwitch433;
void setupRF()
{
	if (rf433InPin >= 1 || rf433OutPin >= 1)
	{
		mySwitch433 = RCSwitch();
	//	mySwitch433.setProtocol(1);
	//	mySwitch433.setRepeatTransmit(15);
		if (rf433InPin >= 1) {
			mySwitch433.enableReceive(digitalPinToInterrupt(rf433InPin));
			Serial.print(F("R RF433 ON "));
			Serial.println(digitalPinToInterrupt(rf433InPin));
		}
		if (rf433OutPin >= 1) {
	//		mySwitch433.enableTransmit(rf433OutPin);
		//	pinMode(rf433OutPin, OUTPUT);
		}
	}
}

void ProcesarRF()
{
	if (rf433InPin >= 1 && mySwitch433.available())
	{
		sendMqttf(strSensores + "/RF433", mySwitch433.getReceivedValue(), false);
		mySwitch433.resetAvailable();
	//	BlinkLedStatus = RFDetected;
	}
}

void ProcesarComandoRF(String topic, String valor)
{
	Serial.println(topic);
	

	String Modo = getValue(topic, '/', 4);
	int nroPin = 0;

	if (Modo == "RAW")
	{
		nroPin = getValue(topic, '/', 5).toInt();
		if (nroPin == 0) nroPin = rf433OutPin;
		String timings = getValue(valor, ':', 0);
		String strBuckets = getValue(valor, ':', 1);
   		unsigned long buckets[8];
		char buffert[60];
		char bufferb[35];
		timings.toCharArray(buffert, 60);
		strBuckets.toCharArray(bufferb, 35);
		Serial.println(nroPin);
		Serial.println(timings);
		Serial.println(strBuckets);	
		int cnt = 0;
        char * pch;
        pch = strtok(bufferb,",");
        buckets[cnt] = atoi(pch);
        while (pch != NULL)
        {
            pch = strtok (NULL, ",");
            cnt++;
            buckets[cnt] = atoi(pch);
        }
		
		RFControl::sendByCompressedTimings(nroPin,buckets,buffert,4);
		
	}
	else
	{
	
		nroPin = getValue(topic, '/', 4).toInt();
		if (nroPin == 0) nroPin = rf433OutPin;
		char buffert[40];
		valor.toCharArray(buffert, 40);
		mySwitch433.enableTransmit(nroPin);
		mySwitch433.send(buffert);
		Serial.println(nroPin);
		Serial.println(valor);
		Serial.println(buffert);
	}
}