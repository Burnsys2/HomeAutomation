float intercept = -0.04; // to be adjusted based on calibration testing
float slope = 0.0405; // to be adjusted based on calibration testing
float testFrequency = 60;                     // test signal frequency (Hz)
float windowLength = 40.0 / testFrequency;     // how long to average the signal, for statistist
unsigned long VoltSensorReportPeriod = 1000; //Refresh rate
int VoltSensor = 0; //Sensor analog input, here it's A0

float current_Volts; // Voltage
unsigned long VoltSensorMillis = 0;
RunningStatistics inputStats;

void SetupVoltSensor()
{
	if (VoltSensorPin <= 0) return;
	inputStats.setWindowSecs(windowLength);
}

void ProcesarVoltSensor()
{
	if (VoltSensorPin <= 0) return;
	VoltSensor = analogRead(VoltSensorPin);  // read the analog in value:
	inputStats.input(VoltSensor);  // log to Stats function
	if ((unsigned long)(millis() - VoltSensorMillis) >= VoltSensorReportPeriod) {
		VoltSensorMillis = millis();
		current_Volts = intercept + slope * inputStats.sigma(); //Calibartions for offset and amplitude
		sendMqttf(F("Sensores/Volts/raw"), current_Volts, false);
		sendMqttf(F("Sensores/Volts/calc"), current_Volts * (40.3231), false);
		sendMqttf(F("Sensores/Volts/sigma"), inputStats.sigma() , false);
	}
}