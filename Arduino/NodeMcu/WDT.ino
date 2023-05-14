int wdtInterval = 4000;
int wdtInterval2 = 200;
long previousMillisWDT = 0;
bool wdtSending = false;
bool wdtEnabled = true;
void ProcesarWDT()
{
	if (WDTPin == -1) return;
	if (!wdtEnabled) return;
  	unsigned long currentMillis = millis();
	if (!wdtSending)
	{
		if(currentMillis - previousMillisWDT < wdtInterval) return;
		pinMode(WDTPin, OUTPUT);
		digitalWrite(WDTPin,HIGH);
		wdtSending = true;
		sendMqttf("WDT","Heartbeat",false);

	}
	else
	{
		if(currentMillis - previousMillisWDT < wdtInterval2) return;
  		digitalWrite(WDTPin, LOW);
  		pinMode(WDTPin, INPUT);
		wdtSending = false;
	}
	previousMillisWDT = currentMillis;   
}
void ProcesarComandoWDT(String topic, String valor)
{

	byte nro = getValue(topic, '/', 4).toInt();
	String Command = getValue(topic, '/', 5);

	if (Command == F("INTERVAL"))  wdtInterval = valor.toInt();
	if (Command == F("STOP")) wdtEnabled = false;
	if (Command == F("START")) wdtEnabled = true;
	
}
