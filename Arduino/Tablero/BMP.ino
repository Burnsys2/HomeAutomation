Adafruit_BMP280 bmp; // I2C
float presion; 
void SetupBMP()
{
    bmp.begin(); // Inicia el sensor
}
void ProcesarBMP()
{
  Serial.println("bMPB");

    presion = bmp.readPressure()/100;
  Serial.println("bMPE");

    if (presion <= 0) return;

    sendMqttf("Sensores/BMP/Presion", presion, false);
    sendMqttfPrec("Sensores/BMP/Temperatura", bmp.readTemperature(), false);
}