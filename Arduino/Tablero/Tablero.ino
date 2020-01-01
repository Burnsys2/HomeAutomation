/*
 Name:		Test_Arduino.ino
 Created:	12/25/2018 6:19:05 PM
 Author:	LIVING
*/

#include <FastLED.h>
#include <Ethernet.h>
#include <Encoder.h>
#define MQTT_SOCKET_TIMEOUT 1
#define MQTT_KEEPALIVE 1
#define MQTT_MAX_PACKET_SIZE 1024 //OJO HAY QUE CAMBIAR EN LA LIBRERIA
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
FASTLED_USING_NAMESPACE
#include <avr/wdt.h>
#include <SimpleTimer.h>
#include <Event.h>
#include <PubSubClient.h>
#include <Filters.h> //Easy library to do the calculations

/*
#include <ir_Lego_PF_BitStreamEncoder.h>
#include <IRremoteInt.h>
*/
#include <IRremote.h>
#include <boarddefs.h>
//#include "DHT.h"
#include <dht_nonblocking.h>
#include <RCSwitch.h>

enum eLedStatus {
  Starting,
  Ok,
  OffLine
};

enum eLedAction {
  None,
  Send,
  ReceiveAction,
  ManualAction,
  IrDetected,
  RFDetected,
  ButtonDetected,
  IrSendAction,
  RelayAction,
  AnalogOnlineAction,
};

enum eWsStripMode {
  AnimationStatic,
  AnimationRainbowWithGlitter,
  AnimationRainbow,
  AnimationConfetti,
  AnimationSinelon,
  AnimationBpm,
  AnimationJuggle,
  AnimationFadeTo
};

eLedStatus CurentLedStatus;
eLedAction BlinkLedStatus;
bool OffLineMode = true;



//IR
//https://forum.arduino.cc/index.php?topic=320732.0
/*const int IR_IN_PIN = -1;
irrecv IRrecv irrecv(IR_IN_PIN);
decode_results results;
*/

//TIMERS
SimpleTimer tSensores;

void(* resetFunc) (void) = 0; //declare reset function @ address 0

void softReset(){
asm volatile ("  jmp 0");
}

SimpleTimer tReconnect2; //no tiene razon de ser
void setup(void)
{
	wdt_disable();
	
	CurentLedStatus = Starting;
	SetupStatusLed();
	SetupRGBLeds();
	Serial.begin(9600);
	while (!Serial) {}
	SetupWsStrips();
	SetLedStatus();
	Serial.println(F("Starting..."));
	setupIR();
	SetupSensores();
	SetupVoltSensor();
	setupButtonsRelays();
	setupRF();
	SetupDHT();
	tSensores.setInterval(5000,TSensoresLentos);
	setupEthernet();
	SetupEncoders();
	wdt_enable(WDTO_8S);
	CurentLedStatus = Starting;
	
	Serial.println(F("Fin Setup"));
}

void TSensoresLentos()
{
    sendMqttf("LastSeen",1,false);
   // reportIp();
	InformarSensores();
	InformarBotonesYRelays();
   	BlinkLedStatus = Send;
	sendMqttf("FreeRam",freeMemory(),false);
}

void loop() {
 	wdt_reset();
  	OffLineMode = !ProcesarRed();

	if (OffLineMode) 
		CurentLedStatus = OffLine;
	else
		CurentLedStatus = Ok;	

	DetectarBotones();
	ProcesarIR();
	ProcesarRF();
	SetLedStatus();
	SetLedAction();
	ProcesarDht();
	ProcesarSensores();
	ProcesarVoltSensor();
	ProcesarWsStrip();
	ProcesarEncoders();
	tSensores.run();
}