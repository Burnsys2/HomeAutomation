const byte DHTSize = sizeof(DHTArray) / sizeof(DHTArray[0]);
unsigned long LastMes[DHTSize];
DHT_nonblocking* DHTs[DHTSize];
void SetupDHT()
{
	Serial.println("DHT");
	for (byte index = 0; index < DHTSize; index++) {
		DHTs[index] = new DHT_nonblocking(DHTArray[index][0], DHTArray[index][1]);
        LastMes[index] = millis();
	}
}

void ProcesarDht()
{
	for (byte index = 0; index < DHTSize; index++) {

        float temperature;
        float humidity;
        /* Measure temperature and humidity.  If the functions returns
           true, then a measurement is available. */
        if (LeerDht(index, &temperature, &humidity) == true)
        {
            sendMqttf("DHT/" + String(index) + "/Temperatura", temperature, true);
            sendMqttf("DHT/" + String(index) + "/Humedad", humidity, true);
        }
	}
}


static bool LeerDht(byte index, float* temperature, float* humidity)
{
    //LastMes[index] = millis();
    //static unsigned long measurement_timestamp = millis();

    /* Measure once every four seconds. */
    if (millis() - LastMes[index] > 4000ul)
    {
        if (DHTs[index]->measure(temperature, humidity) == true)
        {
            LastMes[index] = millis();
            return(true);
        }
    }

    return(false);
}