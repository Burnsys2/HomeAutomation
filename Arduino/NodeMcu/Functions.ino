String getValue(String data, char separator, int index)
{
	int found = 0;
	int strIndex[] = { 0, -1 };
	int maxIndex = data.length();

	for (int i = 0; i <= maxIndex && found <= index; i++) {
		if (data.charAt(i) == separator || i == maxIndex) {
			found++;
			strIndex[0] = strIndex[1] + 1;
			strIndex[1] = (i == maxIndex) ? i + 1 : i;
		}
	}
	return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}  // END


String array_to_string(byte* payload, unsigned int length)
{
	String res = "";
	for (int i = 0; i < length; i++) {
		res = res + (char)payload[i];
	}
	return res;
}

int FindIndex(const byte a[], byte value, int ArraySize)
{
	int index = 0;

	while (index < ArraySize && a[index] != value) ++index;

	return (index == ArraySize ? -1 : index);
}
int FindIndex(const int a[], int value, int ArraySize)
{
	int index = 0;

	while (index < ArraySize && a[index] != value) ++index;

	return (index == ArraySize ? -1 : index);
}
String IpAddress2String(const IPAddress& ipAddress)
{
	return String(ipAddress[0]) + String(".") + String(ipAddress[1]) + String(".") + String(ipAddress[2]) + String(".") + String(ipAddress[3]);
}
int freeMemory() {
	char top;
#ifdef __arm__
	return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
	return &top - __brkval;
#else  // __arm__
	return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}
