#include <IRremote.h>
//CONECTAR AL PIN 2
int RECV_PIN = 31;
IRrecv irrecv(RECV_PIN);
IRsend irsend; // Defaults to pin 3
decode_results results;
unsigned int rawCodes[RAWBUF];
int code[199] ={4300,4400,450,1700,450,600,450,1650,450,1700,450,600,450,600,450,1650,450,600,500,600,450,1650,450,600,450,600,450,1650,450,1650,500,600,450,1650,500,550,450,1700,450,1650,450,600,450,1650,500,550,500,1700,450,1650,500,1650,450,600,450,600,450,1700,450,600,450,1650,500,600,450,600,450,1650,450,1700,450,1650,500,600,450,600,450,600,450,600,450,600,450,600,450,600,450,600,450,1700,450,1700,450,1700,450,1650,450,1650,450,5250,4350,4400,450,1700,450,600,450,1650,450,1650,450,600,450,600,450,1650,450,600,450,600,450,1700,450,600,450,600,450,1700,450,1700,450,600,450,1700,450,600,450,1650,450,1700,450,600,450,1700,450,600,450,1700,450,1700,450,1650,450,600,450,600,450,1700,450,600,450,1700,450,600,450,600,450,1700,450,1700,450,1700,450,600,450,600,450,600,450,600,450,600,450,600,450,600,450,600,450,1700,450,1700,450,1700,450,1700,450,1700,450};
void setup(){
  Serial.begin(9600);
  irrecv.enableIRIn();
  pinMode(9,OUTPUT);
  Serial.println("Info    : Infrared Decoder and Extender");
  Serial.println("Version : 1.0");
  Serial.println("---------------------------------------");
 Serial.print("USECPERTICK: ");
Serial.println(USECPERTICK);
 Serial.print("MARK_EXCESS: ");
Serial.println(MARK_EXCESS);

}

void loop(){
  // IR Extender
  if (irrecv.decode(&results)) {
    Serial.println("RAW");
    //Serial.println(results.value, HEX);
    int codeLen = results.rawlen - 1;
       for (int i = 1; i <= codeLen; i++) {
  //       Serial.print(results.rawbuf[i]); 
  //  Serial.print(","); 
    }
    Serial.println("");
    Serial.println("RAW FINAL PARA MQTT: ");
    for (int i = 1; i <= codeLen; i++) {
      if (i % 2) {
        // Mark
        rawCodes[i - 1] = results.rawbuf[i]*USECPERTICK - MARK_EXCESS;
      } 
      else {
        // Space
        rawCodes[i - 1] = results.rawbuf[i]*USECPERTICK + MARK_EXCESS;
        }
      Serial.print(rawCodes[i - 1]); 
    Serial.print(","); 
    }

      irsend.sendRaw(code, 199, 38);
     Serial.println();
    Serial.println(codeLen);
    irrecv.enableIRIn();
    irrecv.resume(); // Receive the next value
  //  dump(&results);
    //Serial.print(rawCodes[i - 1], DEC);
  }
}

// Dump results
void dump(decode_results *results) {
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  } 
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");
  } 
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  } 
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  } 
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
   else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 0; i < count; i++) {
    if ((i % 2) == 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    } 
    else {
      Serial.print(-(int)results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println("");
}
