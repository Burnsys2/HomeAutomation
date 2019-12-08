
//RCSwitch mySwitch315;
RCSwitch mySwitch433;


void setupRF()
{
	/*if (rf315InPin >= 1 || rf315OutPin >= 1)
	{
		mySwitch315	= RCSwitch();
		mySwitch315.setProtocol(1);
		if (rf315InPin >= 1 ) {	
			mySwitch315.enableReceive(digitalPinToInterrupt(rf315InPin));
			Serial.print(F("Receiving RF315 ON "));
			Serial.println(digitalPinToInterrupt(rf315InPin));

			 }
		if (rf315OutPin >= 1 ) {
			mySwitch315.enableTransmit(rf315OutPin); 
			Serial.print(F("Transmiting RF315 ON "));
			Serial.println(rf315OutPin);
			pinMode(rf315OutPin,OUTPUT);
		}
	}*/

	if (rf433InPin >= 1 || rf433OutPin >= 1)
	{
		mySwitch433	= RCSwitch();
		mySwitch433.setProtocol(1);
		if (rf433InPin >= 1 ) {	
			mySwitch433.enableReceive(digitalPinToInterrupt(rf433InPin)); 
			Serial.print(F("Receiving RF433 ON "));
			Serial.println(digitalPinToInterrupt(rf433InPin));
		}
		if (rf433OutPin >= 1 ) {
			mySwitch433.enableTransmit(rf433OutPin); 
			pinMode(rf433OutPin,OUTPUT);
			}
	}
}

void ProcesarRF()
{
 //   if (rf315InPin >= 1 && mySwitch315.available()) 
	//{
	//    Serial.print(F(" microseconds"));
	////	output(mySwitch315.getReceivedValue(), mySwitch315.getReceivedBitlength(), mySwitch315.getReceivedDelay(), mySwitch315.getReceivedRawdata(),mySwitch315.getReceivedProtocol());
	//	sendMqttf(F("Sensores/RF315"),mySwitch315.getReceivedValue(),false);
 // 		mySwitch315.resetAvailable();
	//	BlinkLedStatus = RFDetected;
	//  }
	  
	if (rf433InPin >= 1 && mySwitch433.available()) 
	{
	//	output(mySwitch433.getReceivedValue(), mySwitch433.getReceivedBitlength(), mySwitch433.getReceivedDelay(), mySwitch433.getReceivedRawdata(),mySwitch433.getReceivedProtocol());
		sendMqttf(F("Sensores/RF433"),mySwitch433.getReceivedValue(),false);
		mySwitch433.resetAvailable();
		BlinkLedStatus = RFDetected;
  	}
	  
}

void ProcesarComandoRF(String topic, String valor)
{
    String tipo = getValue(topic,'/',3);
  	char buffert[25];
	valor.toCharArray(buffert,25);

    if (tipo == F("RF433"))
    {
	    mySwitch433.send(buffert);
    }

    if (tipo == F("RF315"))
    {
		mySwitch433.send(buffert);
    }
	
  //  Serial.print("RF >>> ");
//    Serial.print(valor);
//    Serial.println(buffert);
    }