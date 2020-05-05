int analogInsOnlineMaxDev = 50;
int analogInsOnlineMilis = 100;
const byte analogInsArraySize = sizeof(analogInsArray)/sizeof(analogInsArray[0]);
const byte analogInsOnlineArraySize = sizeof(analogInsOnlineArray)/sizeof(analogInsOnlineArray[0]);
long analogInsValue[analogInsArraySize];
long analogInsMaxValue[analogInsArraySize];
int analogInsOnlineValue[analogInsOnlineArraySize];
int analogInsOnlineEMA_S[analogInsOnlineArraySize];
float analogInsOnlineEMA_A[analogInsOnlineArraySize];
long analogInsOnlineLastReport[analogInsOnlineArraySize];

int analogInsSamples[analogInsArraySize];

void SetupSensores()
{
	for (byte index = 0; index < analogInsArraySize; index++) 
	{
		analogInsValue[index]= 0;
		analogInsMaxValue[index]= 0;
		analogInsSamples[index]= 0;
	}

	for (byte index = 0; index < analogInsOnlineArraySize; index++) 
	{
		analogInsOnlineValue[index] = 0;
		analogInsOnlineEMA_S[index] = analogRead(analogInsOnlineArray[index]);
		analogInsOnlineEMA_A[index] = 0.6;
		analogInsOnlineLastReport[index] = millis();
	 	}
}

void ProcesarSensores()
{
	for (byte index = 0; index < analogInsArraySize; index++) 
	{

		analogInsSamples[index] += 1;
		int valor =  analogRead(analogInsArray[index]);
		analogInsValue[index] += valor;
		if (analogInsMaxValue[index] < valor)
		{
			analogInsMaxValue[index] = valor;
		}
	}
	
	for (byte index = 0; index < analogInsOnlineArraySize; index++) 
	{
	   	int analogR = analogRead(analogInsOnlineArray[index]);
		if (analogR == analogInsOnlineValue[index]) {continue;}
		analogInsOnlineEMA_S[index] = (analogInsOnlineEMA_A[index] * analogR) + ((1-analogInsOnlineEMA_A[index])*analogInsOnlineEMA_S[index]);
		analogR = analogInsOnlineEMA_S[index];
		if (analogInsOnlineEMA_S[index] == analogInsOnlineValue[index]) {continue;}
		if (abs(analogInsOnlineValue[index] - analogR) < analogInsOnlineMaxDev && analogR != 0 && analogR < 1023) {continue;}
		if (abs(millis() - analogInsOnlineLastReport[index]) < analogInsOnlineMilis) {continue;}
	
		sendMqttf(strSensores + "/Analog/" + String(analogInsOnlineArray[index]) ,analogR,true);
		analogInsOnlineValue[index] = analogR;
		analogInsOnlineLastReport[index] = millis();
	}
}

void ProcesarComandoSensores(String topic, String valor)
{
	if (getValue(topic,'/',4) == F("MAXDEV"))
	{
		analogInsOnlineMaxDev = valor.toInt();
		return;
	}
	if (getValue(topic,'/',4) == F("MILIS"))
	{
		analogInsOnlineMilis = valor.toInt();
		return;
	}
}
void InformarSensores()
{
//	Serial.println("InformarSensores");
	for (byte index = 0; index < analogInsArraySize; index++) 
	{
		if (analogInsSamples[index] > 0)
		{
			sendMqttf(strSensores + "/Analog/" + String(analogInsArray[index]) ,analogInsValue[index] / analogInsSamples[index],true);
			sendMqttf(strSensores + "/Analog/Max/" + String(analogInsArray[index]) ,analogInsMaxValue[index] ,true);
	//		sendMqttf("Sensores/Analog/Samples/" + String(analogInsArray[index]) ,analogInsSamples[index],true);
	//		sendMqttf("Sensores/Analog/Total/" + String(analogInsArray[index]) ,analogInsValue[index],true);
		}
		analogInsValue[index] = 0;
		analogInsSamples[index] = 0;
		analogInsMaxValue[index] = 0;
	}
	
	for (byte index = 0; index < analogInsOnlineArraySize; index++) 
	{
	   	int analogR = analogRead(analogInsOnlineArray[index]);
		if (analogR == analogInsOnlineValue[index]) {continue;}
		sendMqttf(strSensores + "/Analog/" + String(analogInsOnlineArray[index]) ,analogR,true);
		analogInsOnlineValue[index] = analogR;
		analogInsOnlineLastReport[index] = millis();
	}
}