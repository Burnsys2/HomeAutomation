//botones
const byte buttonIgnoreNoiseFilterArraySize = sizeof(buttonIgnoreNoiseFilterArray)/sizeof(buttonIgnoreNoiseFilterArray[0]);
const byte buttonArraySize = sizeof(buttonArray)/sizeof(buttonArray[0]);
int buttonState[buttonArraySize];
unsigned long buttonStateMilis[buttonArraySize];
int noiseMillis = 100;

//relays
const byte RelaysArraySize = sizeof(RelaysArray)/sizeof(RelaysArray[0]);
int relayForced[RelaysArraySize];

const byte RelaysManualMapSize = sizeof(RelaysManualMap)/sizeof(RelaysManualMap[0]);
const byte RelaysInvertedArraySize = sizeof(RelaysInvertedArray)/sizeof(RelaysInvertedArray[0]);
	
void setupButtonsRelays()
{
	//Serial.println(F("setupButtonsRelays"));

	for (byte index = 0; index < buttonArraySize; index++) {
   		 pinMode(buttonArray[index], INPUT);	
		 buttonState[index] = digitalRead(buttonArray[index]); //para que no salte el toggle al reiniciar
		 buttonStateMilis[index] = 0; //para que no salte el toggle al reiniciar
  	}
    for (byte index = 0; index < RelaysArraySize; index++) {
   		 pinMode(RelaysArray[index], OUTPUT);
		 digitalWrite(RelaysArray[index],HIGH); //todos los relays en off
     	 relayForced[index] = -1;
  	}
}

void InformarBotonesYRelays()
{
	for (byte index = 0; index < buttonArraySize; index++) {
   		sendMqttf(strButton + "/" + String(buttonArray[index]), digitalRead(buttonArray[index]),true);
  	}
    
    for (byte index = 0; index < RelaysArraySize; index++) {
   		InformarRelay(index);
//   		sendMqttf("Relay/Forced/" + String(RelaysArray[index]), relayForced[index] ,true);
  	}
}
void InformarRelay(byte index)
{
	int estado = digitalRead(RelaysArray[index]);
	//invertir estado si es inverted
	if (FindIndex(RelaysInvertedArray,RelaysArray[index],RelaysInvertedArraySize) != -1)
		{if (estado == HIGH)	{estado = LOW; } else {estado = HIGH;}}

	if (estado == HIGH)	{estado = 0; } else {estado = 1;}
		
	sendMqttf("Relay/" + String(RelaysArray[index]),  estado ,true);
	sendMqttf("Relay/Forced/" + String(RelaysArray[index]), relayForced[index] ,true);
}
void DetectarBotones()
{
		for (byte index = 0; index < buttonArraySize; index++) {
			int currPin = digitalRead(buttonArray[index]);
		
			if (buttonState[index] == currPin) 
			{
				buttonStateMilis[index] = 0; //resetear ruido
				continue;
			} 

			if (buttonState[index] != currPin) //delay para evitar ruido
			{
			//	BlinkLedStatus = ButtonDetected;
				if (buttonStateMilis[index] == 0)
				{
					buttonStateMilis[index] = millis();
				}
				if (millis() - buttonStateMilis[index] < noiseMillis) {continue;}

				Serial.println(OffLineMode);
				//reseteo millis
				buttonStateMilis[index] = 0;
				buttonState[index] = currPin;
                if (OffLineMode)
                {
					//BlinkLedStatus = ManualAction;
                    for ( byte i = 0; i < RelaysManualMapSize; i++) {
                        if ( RelaysManualMap[i][0] == buttonArray[index] )  //ES EL BOTON
                        {
							Serial.println(buttonArray[index]);
                            setRelayState(RelaysManualMap[i][1],F("TOGGLE"));
                        }
                    }
                }
				sendMqttf(strButton + "/" + String(buttonArray[index]) + "/toggle", currPin,false);
				InformarBotonesYRelays(); //informar solo actual
			}
  		}
}

void ProcesarComandoRelays(String topic, String valor)
{
	if (getValue(topic,'/',4) == F("NOISEMILLIS"))
	{
		noiseMillis = valor.toInt();
		return;
	}

	byte nro = getValue(topic,'/',4).toInt();
    byte index = FindIndex(RelaysArray,nro,RelaysArraySize);
    
	if (valor == F("FORCE-1") || valor == F("FORCE-ON"))
        {relayForced[index] = 1;	}
    if (valor == F("FORCE-0") || valor == F("FORCE-OFF"))
        {relayForced[index] = 0;	}
    if (valor == F("UNFORCE"))
        {relayForced[index] = -1;	}

    if (relayForced[index] == -1) //SI NO ESTA FORZADO MANDO LA SEÑAN
        {setRelayState(nro,valor);}		
    else
        {setRelayState(nro,String(relayForced[index]));}
}

void setRelayState(byte nro, String estado)
{
	int estadoTmp = -1;
	if (estado == "0" || estado == F("OFF") || estado == F("FALSE"))	{estadoTmp = HIGH;}
	if (estado == "1" || estado == F("ON")  || estado == F("TRUE"))	{estadoTmp = LOW;}

	if (estadoTmp != -1)
	{
	//invertir estado si es inverted
		if (FindIndex(RelaysInvertedArray,nro,RelaysInvertedArraySize) != -1)
			{if (estadoTmp == HIGH)	{estadoTmp = LOW; } else {estadoTmp = HIGH;}}	
		digitalWrite(nro,estadoTmp);
	}

	if (estado == F("TOGGLE"))
	{
			int last = digitalRead(nro);
			if (last == HIGH)
				{digitalWrite(nro,LOW);}
			else
				{digitalWrite(nro,HIGH);}
	}
	InformarRelay(FindIndex(RelaysArray,nro,RelaysArraySize));
//		sendMqttfAsBool("Relay/" + String(nro), digitalRead(nro) * -1,true);
  // 		sendMqttf("Relay/Forced/" + String(nro), relayForced[FindIndex(RelaysArray,nro,RelaysArraySize)],true );
}


