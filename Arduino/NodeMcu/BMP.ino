Adafruit_BMP280 bmp; // I2C
float presion; 
void SetupBMP()
{
    if (!BMP280Present) return;
    bmp.begin(); // Inicia el sensor

}
void ProcesarBMP()
{
    
    if (!BMP280Present) return;
    presion = bmp.readPressure()/100;
    if (presion <= 0) return;
    sendMqttf("Sensores/BMP/Presion", presion, false);
    sendMqttfPrec("Sensores/BMP/Temperatura", bmp.readTemperature(), false);
    
}