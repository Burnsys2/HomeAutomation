/*
  Example for different sending methods
  
  https://github.com/sui77/rc-switch/
  
*/

#include <RCSwitch.h>

RCSwitch mySwitch = RCSwitch();

void setup() {

  Serial.begin(9600);
  
  // Transmitter is connected to Arduino Pin #10  
  Serial.print("test");
  mySwitch.enableTransmit(18);

  // Optional set pulse length.
   mySwitch.setPulseLength(320);
  
  // Optional set protocol (default is 1, will work for most outlets)
  // mySwitch.setProtocol(2);
  
  // Optional set number of transmission repetitions.
   mySwitch.setRepeatTransmit(15);
  
}

void loop() {
  /* Same switch as above, but using binary code */
 // mySwitch.send("1011011100011011000001"); //Control 1 A
 // mySwitch.send("1011011100011011000010");  //Control 1 B
  mySwitch.send("100000101100100011001000");  //Control 1 C
  //mySwitch.send("1011011100011011001010"); //Control 1 D
  Serial.print("SEND");
    delay(3000);  

 // mySwitch.send("1011011100011011001011"); //Control 2 A
 // mySwitch.send("1011011100011011001100");  //Control 2 B
  //mySwitch.send("1011011100011011001101");  //Control 2 C
  mySwitch.send("10011000100101101000001"); //Control 2 D
  
  delay(3000);  


}
