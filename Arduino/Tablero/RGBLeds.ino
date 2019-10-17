const byte RGBLedsSize = sizeof(RGBLeds)/sizeof(RGBLeds[0]);

void SetupRGBLeds()
{
	for (byte index = 0; index < RGBLedsSize; index++) {
    	for (byte z = 0; z < 3; z++) {
            pinMode(RGBLeds[index][z], OUTPUT);
        }
  	}
}
void ProcesarComandoLedsRgb(String topic, String valor)
{
    byte nro = getValue(topic,'/',4).toInt();
    for(byte i = 0; i < 3; i++)
    {
        analogWrite(RGBLeds[nro][i],255 - getValue(valor,',',i).toInt()) ;
    }
}