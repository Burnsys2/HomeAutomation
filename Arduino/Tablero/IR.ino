IRrecv irrecv(IrRecPin);
IRsend irsend; // usar pin 9 en mega
decode_results results;

void setupIR()
{
    if (IrRecPin > 0)
    {
        irrecv.enableIRIn();
    }
}
void ProcesarIR()
{
     if (IrRecPin < 0) {return;}

    if (irrecv.decode(&results)) {
     //   BlinkLedStatus = IrDetected;
    //    Serial.print("HEX:");
      //  Serial.println(results.value, HEX);
       // Serial.println(results.value);
        sendMqttf(F("Sensores/Ir"),results.value,false);
        irrecv.enableIRIn();
        irrecv.resume(); //
    }

}

void ProcesarComandoIR(String topic, byte* payload,unsigned int length)
{
    String tipo = getValue(topic,'/',4);
  	char str[length];
    for (int i=0;i<length;i++) {
        str[i] =  (char)payload[i];
    }
    if (tipo == F("RAW"))
    {
        unsigned int rawCodes[800];
        int cnt = 0;
        char * pch;
        pch = strtok (str,",");
        rawCodes[cnt] = atoi(pch);
        while (pch != NULL)
        {
            pch = strtok (NULL, ",");
            cnt++;
            rawCodes[cnt] = atoi(pch);
        }

        for (int i = 0; i < cnt; i++) {
            if (i % 2) {
            // Mark
                rawCodes[i] = rawCodes[i]*50 + 100;
            } 
            else {
            // Space
                rawCodes[i] = rawCodes[i]*50 - 100;
            }
        }
        irsend.sendRaw(rawCodes, cnt, 38);
    }
}