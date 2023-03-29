
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
//		AiEsp32RotaryEncoder *Enc = Encoders[index];
		Encoders[index]->readEncoder_ISR();
	}
}
void SetupEncoders()
{
	Serial.println("Encoders");
	for (byte index = 0; index < EncodersSize; index++) 
	{
		Encoders[index] = new AiEsp32RotaryEncoder(EncodersPins[index][0], EncodersPins[index][1],EncodersPins[index][2],-1,1);
//   		pinMode(EncodersPins[index][0], INPUT);	
 //  		pinMode(EncodersPins[index][1], INPUT);	

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
/*

		if (NewValue == LastEncPosition[index]) continue;
		if (NewValue > LastEncPosition[index]) 	NewValue = LastEncPosition[index] + 1;
		if (NewValue < LastEncPosition[index]) 	NewValue = LastEncPosition[index] - 1;

		if (NewValue > EncMax[index])
		{
			if (EncLoop[index] == true)
				NewValue = EncMin[index] + NewValue - LastEncPosition[index];
			else
				NewValue = EncMax[index];
		}
		if (NewValue < EncMin[index])
		{
			if (EncLoop[index] == true)
				NewValue = EncMax[index] - NewValue - LastEncPosition[index];
			else
				NewValue = EncMin[index];
		}
		Enc->write(NewValue);
	//	Serial.println("-");
		if (NewValue > LastEncPosition[index]) 	sendMqttf("Encoder/" + String(index), "+", false);
		if (NewValue < LastEncPosition[index])	sendMqttf("Encoder/" + String(index) , "-", false);
		sendMqttf("Encoder/" + String(index) + "/value", NewValue, true);
		LastEncPosition[index] = NewValue;
*/
	}
}

void ReportEncoders()
{
	/*
	for (byte index = 0; index < EncodersSize; index++) {
		int NewValue = 0;
		AiEsp32RotaryEncoder *Enc = Encoders[index];
		NewValue = Enc->read();
		sendMqttf("Encoder/" + String(index) + "/value", NewValue, true);
	}
	*/
}
void ProcesarComandoEncloders(String topic, String valor)
{

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
	if (Command == F("REPORT")) ReportEncoders();
	
	Encoders[nro]->setBoundaries(EncMin[nro], EncMax[nro], EncLoop[nro]); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
	if (Command == F("ACCELL")) 
	{
    	Encoders[nro]->setAcceleration(valor.toInt());

	}
}
