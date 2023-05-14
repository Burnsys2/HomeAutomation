
const byte EncodersSize = sizeof(EncodersPins) / sizeof(EncodersPins[0]);
AiEsp32RotaryEncoder *Encoders[EncodersSize];
int LastEncPosition[EncodersSize];
int EncMax[EncodersSize];
int EncMin[EncodersSize];
bool EncLoop[EncodersSize];
void IRAM_ATTR readEncoderISR()
{
 	for (byte index = 0; index < EncodersSize; index++) 
	{
		Encoders[index]->readEncoder_ISR();
	}
}
void SetupEncoders()
{
	Serial.println("Encoders");
	for (byte index = 0; index < EncodersSize; index++) 
	{
		Encoders[index] = new AiEsp32RotaryEncoder(EncodersPins[index][0], EncodersPins[index][1],EncodersPins[index][2],-1,1);

		Encoders[index]->begin();
		Encoders[index]->setup(readEncoderISR);

		LastEncPosition[index] = -999;
		EncMin[index] = -999;
		EncMax[index] = 999;
		EncLoop[index] = false;

  	  	Encoders[index]->setBoundaries(EncMin[index],EncMax[index], EncLoop[index]); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
    	Encoders[index]->setAcceleration(250);
	}
}


void ProcesarEncoders()
{
	for (byte index = 0; index < EncodersSize; index++) {
		int NewValue = 0;

    	if (Encoders[index]->encoderChanged())
		{
			NewValue = Encoders[index]->readEncoder();
			Serial.println(NewValue);
			sendMqttf("Encoder/" + String(index) + "/value", NewValue, true);

		}
	}
}
void ProcesarComandoEncloders(String topic, String valor)
{

	if (EncodersSize == 0) return;
	byte nro = getValue(topic, '/', 4).toInt();
	String Command = getValue(topic, '/', 5);

	if (Command == F("VALUE"))
	{
		LastEncPosition[nro] = valor.toInt();
		Encoders[nro]->reset(valor.toInt());
		sendMqttf("Encoder/" + String(nro) + "/value", valor, true);
	}
	if (Command == F("MIN")) EncMin[nro] = valor.toInt();
	if (Command == F("MAX")) EncMax[nro] = valor.toInt();
	if (Command == F("LOOP")) EncLoop [nro] = valor.toInt();
	
	Encoders[nro]->setBoundaries(EncMin[nro], EncMax[nro], EncLoop[nro]); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
	if (Command == F("ACCELL")) 
	{
    	Encoders[nro]->setAcceleration(valor.toInt());

	}
}
