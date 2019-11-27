IRsend irsend(4); // usar D2
void setupIR()
{
	irsend.begin();
}
void ProcesarComandoIR(String topic, byte* payload, unsigned int length)
{
	String tipo = getValue(topic, '/', 4);
	char str[length];
	for (int i = 0; i < length; i++) {
		str[i] = (char)payload[i];
	}
	if (tipo == F("RAW"))
	{
		uint16_t rawCodes[800];
		int cnt = 0;
		char* pch;
		pch = strtok(str, ",");
		rawCodes[cnt] = atoi(pch);
		while (pch != NULL)
		{
			pch = strtok(NULL, ",");
			if (pch == NULL) break;
			cnt++;
			rawCodes[cnt] = atoi(pch);
		}

		for (int i = 0; i < cnt; i++) {
			if (i % 2) {
				// Mark
				rawCodes[i] = rawCodes[i] * 50 + 100;
			}
			else {
				// Space
				rawCodes[i] = rawCodes[i] * 50 - 100;
			}
		}
		irsend.sendRaw(rawCodes, cnt, 38);
	}
}
//void ProcesarComandoIR(String topic, byte* payload,unsigned int length)
//{
//	Serial.println(F("IR"));
//	String tipo = getValue(topic,'/',4);
//	Serial.println(length);
//
//	//char str[length];
//	//Serial.println(str);
//	String str2 = array_to_string(payload, length);
////	Serial.println(valor);
////    for (int i=0;i<length;i++) {
////        str[i] =  (char)payload[i];
////    }
//	Serial.println(str2);
//
//	char str[length];
//	strcpy(str, str2.c_str());	// or pass
//	Serial.println(F("IR2"));
//    if (tipo == F("RAW"))
//    {
//		uint16_t rawCodes[800];
//        int cnt = 0; 
//        char * pch;
//        pch = strtok (str,",");
//        rawCodes[cnt] = atoi(pch);
//		Serial.println(F("IR3"));
//
//        while (pch != NULL)
//        {
//            pch = strtok(NULL, ",");
//			Serial.print(cnt);
//			cnt++;
//			if (pch == NULL) break;
//            rawCodes[cnt] = atoi(pch);
//			Serial.print("-");
//
//			Serial.println(cnt);
//
//		}
//		Serial.println(F("IR4"));
//
//        for (int i = 0; i < cnt; i++) {
//            if (i % 2) {
//            // Mark
//                rawCodes[i] = rawCodes[i]*50 + 100;
//            } 
//            else {
//            // Space
//                rawCodes[i] = rawCodes[i]*50 - 100;
//            }
//        }
//		Serial.println(F("DECODED"));
//        irsend.sendRaw(rawCodes, cnt, 38);
//		Serial.println(F("SENDED"));
//    }
//}