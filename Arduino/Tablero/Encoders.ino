const byte EncodersSize = sizeof(EncodersPins) / sizeof(EncodersPins[0]);
Encoder *Encoders[EncodersSize];
int LastEncPosition[EncodersSize];
int EncMax[EncodersSize];
int EncMin[EncodersSize];
bool EncLoop[EncodersSize];
void SetupEncoders()
{
	//Serial.println("Encoders");
	for (byte index = 0; index < EncodersSize; index++) {
		Encoders[index] = new Encoder(EncodersPins[index][0], EncodersPins[index][1]);
   		pinMode(EncodersPins[index][0], INPUT);	
   		pinMode(EncodersPins[index][1], INPUT);	
		LastEncPosition[index] = -999;
		EncMin[index] = -999;
		EncMax[index] = 999;
		EncLoop[index] = false;
	}
}

void ProcesarEncoders()
{
	for (byte index = 0; index < EncodersSize; index++) {
		int NewValue = 0;
		Encoder *Enc = Encoders[index];
		NewValue = Enc->read();
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
	}
}
void ProcesarComandoEncloders(String topic, String valor)
{

	byte nro = getValue(topic, '/', 4).toInt();
	String Command = getValue(topic, '/', 5);

	if (Command == F("VALUE"))
	{
		Encoder *Enc = Encoders[nro];
		LastEncPosition[nro] = valor.toInt();
		Enc->write(valor.toInt());
	//	sendMqttf("Encoder/" + String(nro) + "/value", valor, true);
	}
	if (Command == F("MIN")) EncMin[nro] = valor.toInt();
	if (Command == F("MAX")) EncMax[nro] = valor.toInt();
	if (Command == F("LOOP")) EncLoop [nro] = valor.toInt();
}