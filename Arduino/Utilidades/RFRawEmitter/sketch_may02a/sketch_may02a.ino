#include <RFControl.h>

void setup() {
  Serial.begin(9600);

}

void loop() {
 Serial.println("-");
    String timings;
    timings = "01100101011001010110100101101010010101010110101002";
    char buff[52];
    timings.toCharArray(buff,52);
        unsigned long buckets[8];
buckets[0]=288;
buckets[1]=652;
buckets[2]=6452;
buckets[3]=0;
buckets[4]=0;
buckets[5]=0;
buckets[6]=0;
buckets[7]=0;       
RFControl::sendByCompressedTimings(5,buckets,buff,4);

delay(2000);
  }
