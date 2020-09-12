String getValue(String data, char separator, byte index)
{
    byte found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length();

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}  // END


String array_to_string(byte* payload , unsigned int length)
{
    String res = "";
  for (int i=0;i<length;i++) {
      res = res +  (char)payload[i];
  }
  return res;
}

int FindIndex( const byte a[], byte value , int ArraySize )
{
    int index = 0;

    while ( index < ArraySize && a[index] != value ) ++index;

    return ( index == ArraySize ? -1 : index );
}
int FindIndex( const int a[], int value , int ArraySize )
{
    int index = 0;

    while ( index < ArraySize && a[index] != value ) ++index;

    return ( index == ArraySize ? -1 : index );
}
