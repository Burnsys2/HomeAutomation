//const byte EncodersSize = sizeof(EncodersPins) / sizeof(EncodersPins[0]);
//Encoder * Encoders[EncodersSize];
//long LastEncPosition[EncodersSize];
//
long positionLeft = -999;
	Encoder knobLeft(18, 19);

void SetupEncoders()
{
/*
	for (byte index = 0; index < EncodersSize; index++) {
		Encoders[EncodersSize] = new Encoder(EncodersPins[index][0], EncodersPins[index][1]);
		LastEncPosition[index] = 0;
	}*/
}


void ProcesarEncoders()
{
	long newLeft, newRight;
	newLeft = knobLeft.read();
	if (newLeft != positionLeft) {
		Serial.print("Left = ");
		Serial.println(newLeft);
		positionLeft = newLeft;
	}
	// if a character is sent from the serial monitor,
	// reset both back to zero.
	if (Serial.available()) {
		Serial.read();
		Serial.println("Reset both knobs to zero");
		knobLeft.write(0);
	}
	/*for (byte index = 0; index < EncodersSize; index++) {
		Encoder *enc = Encoders[index];
		long NewValue = enc->read();
		if (NewValue > LastEncPosition[index])
		{
			sendMqttf("Encoder/" + String(index) + "/CW", NewValue, true);
		}
		if (NewValue < LastEncPosition[index])
		{
			sendMqttf("Encoder/" + String(index) + "/CCW", NewValue, true);
		}
		LastEncPosition[index] = NewValue;
	}*/
}