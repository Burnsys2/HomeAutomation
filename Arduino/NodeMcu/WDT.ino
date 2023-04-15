int wdtTime = 2333;


void ProcesarWDT()
{

}
void ProcesarComandoWDT(String topic, String valor)
{

	byte nro = getValue(topic, '/', 4).toInt();
	String Command = getValue(topic, '/', 5);

	if (Command == F("TIME"))
	{
		LastEncPosition[nro] = valor.toInt();
		Encoders[nro]->reset(valor.toInt());
		sendMqttf("Encoder/" + String(nro) + "/value", valor, true);
	}
	if (Command == F("STOP")) EncMin[nro] = valor.toInt();
	if (Command == F("START")) EncMax[nro] = valor.toInt();
	if (Command == F("LOOP")) EncLoop [nro] = valor.toInt();
	
	Encoders[nro]->setBoundaries(EncMin[nro], EncMax[nro], EncLoop[nro]); //minValue, maxValue, circleValues true|false (when max go to min and vice versa)
	if (Command == F("ACCELL")) 
	{
    	Encoders[nro]->setAcceleration(valor.toInt());

	}
}
