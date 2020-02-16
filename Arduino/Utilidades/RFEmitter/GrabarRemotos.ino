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
  mySwitch.enableTransmit(3);

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
  mySwitch.send("100000101100100011010011");  //Control 1 C
  //mySwitch.send("1011011100011011001010"); //Control 1 D
  Serial.print("SEND");
 // mySwitch.send("1011011100011011001011"); //Control 2 A
 // mySwitch.send("1011011100011011001100");  //Control 2 B
  //mySwitch.send("1011011100011011001101");  //Control 2 C
//mySwitch.send("1011011100011011001111"); //Control 2 D
  
  delay(1000);  


}
