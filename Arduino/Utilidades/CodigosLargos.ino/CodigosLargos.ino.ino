/*
Author: AnalysIR
Revision: 1.0

This code is provided to overcome an issue with Arduino IR libraries
It allows you to capture raw timings for signals longer than 255 marks & spaces.
Typical use case is for long Air conditioner signals.

You can use the output to plug back into IRremote, to resend the signal.

This Software was written by AnalysIR.

Usage: Free to use, subject to conditions posted on blog below.
Please credit AnalysIR and provide a link to our website/blog, where possible.

Copyright AnalysIR 2014

Please refer to the blog posting for conditions associated with use.
http://www.analysir.com/blog/2014/03/19/air-conditioners-problems-recording-long-infrared-remote-control-signals-arduino/

Connections:
IR Receiver      Arduino
V+          ->  +5v
GND          ->  GND
Signal Out   ->  Digital Pin 2
(If using a 3V Arduino, you may connect V+ to +3V)
*/

#define LEDPIN 2// 13
//you may increase this value on Arduinos with greater than 2k SRAM
#define maxLen 250
unsigned int rawCodes[250];
volatile  unsigned int irBuffer[maxLen]; //stores timings - volatile because changed by ISR
volatile unsigned int x = 0; //Pointer thru irBuffer - volatile because changed by ISR

void setup() {
  Serial.begin(9600); //change BAUD rate as required
  attachInterrupt(0, rxIR_Interrupt_Handler, CHANGE);//set up ISR for receiving IR signal
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println(F("IR"));
  delay(5000); // pause 5 secs
  if (x) { //if a signal is captured
    digitalWrite(LEDPIN, HIGH);//visual indicator that signal received
    Serial.println();
    //Serial.print(F("Raw: (")); //dump raw header format - for library
    Serial.print((x - 1));
    //Serial.print(F(") "));
    detachInterrupt(0);//stop interrupts & capture until finshed here
    for (int i = 1; i < x; i++) { //now dump the times
      //if (!(i & 0x1)) Serial.print(F("-"));
     // Serial.print(irBuffer[i] - irBuffer[i - 1]);
      
      rawCodes[i] = (irBuffer[i] - irBuffer[i - 1]) / 50;
      if (rawCodes[i] <0) 
      {rawCodes[i] = rawCodes[i] * -1;}
    //  Serial.print(F(", "));
    }
    
    Serial.println();
    Serial.println();
    digitalWrite(LEDPIN, LOW);//end of visual indicator, for this time

for (int i = 1; i < x; i++) {
   Serial.print(rawCodes[i]);
   Serial.print(",");
}
    Serial.println();
    for (int i = 1; i < x; i++) {
    
       if (i % 2) {
        // Mark
        rawCodes[i] = rawCodes[i]*50 - 100;
      } 
      else {
        // Space
        rawCodes[i] = rawCodes[i]*50 + 100;
        }
      
      Serial.print(rawCodes[i]); 
    Serial.print(","); 
    }
    x = 0;
    attachInterrupt(0, rxIR_Interrupt_Handler, CHANGE);//re-enable ISR for receiving IR signal
  }

}

void rxIR_Interrupt_Handler() {
  if (x > maxLen) return; //ignore if irBuffer is already full
  irBuffer[x++] = micros(); //just continually record the time-stamp of signal transitions

}
