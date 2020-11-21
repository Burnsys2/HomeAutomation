/*
 Name:		Test_Arduino.ino
 Created:	12/25/2018 6:19:05 PM
 Author:	LIVING
*/
//#define FASTLED_ESP8266_RAW_PIN_ORDER
#include <Arduino.h>
//#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER
#define FASTLED_INTERRUPT_RETRY_COUNT 0 
#define FASTLED_ALLOW_INTERRUPTS 0
#define INTERRUPT_THRESHOLD 1
//#define FASTLED_INTERNAL
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#include <IRremoteESP8266.h>
#include "OneButton.h"
#include <IRsend.h>
//para que funcione fastled comentar https://github.com/FastLED/FastLED/blob/403464a499a8feffa48f6f85d205550b9bc9c89b/platforms/esp/8266/led_sysdefs_esp8266.h#L15
#define MQTT_SOCKET_TIMEOUT 1
#define MQTT_KEEPALIVE 1
#define MQTT_MAX_PACKET_SIZE 1024 //OJO HAY QUE CAMBIAR EN LA LIBRERIA
#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char* __brkval;
#endif  // __arm__
#include <ESP8266WiFi.h> 
#include <PubSubClient.h>
#include <SimpleTimer.h>
#include <DHT.h>
#include <ArduinoOTA.h>
#include <SimpleTimer.h>

//#include <IRsend.h>

//#define NUM_LEDS 60

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
	ButtonDetected
};

enum eWsStripMode {
  AnimationStatic,
  AnimationRainbowWithGlitter,
  AnimationRainbow,
  AnimationConfetti,
  AnimationSinelon,
  AnimationStrobe,
  AnimationBpm,
  AnimationJuggle,
  AnimationFadeTo,
  AnimationHueSwipe,
  AnimationRainbowSpin,
  AnimationSerial
};
String strSensores, strButton;

eLedStatus CurentLedStatus;
eLedAction BlinkLedStatus;
bool OffLineMode = true;

//TIMERS
//SimpleTimer tSensores;

void(*resetFunc) (void) = 0; //declare reset function @ address 0
long previousMillis = 0;
long interval = 5000;
void setup() {
//	CurentLedStatus =
	strSensores = F("Sensores");
	strButton = F("Button"); Starting;
	//SetupStatusLed();
//	SetLedStatus();

	Serial.begin(115200);
	while (!Serial) {}
	SetupWsStrips();
	setupIR();
//	setupPushButtons();
	setupEthernet();
	SetupSensores();
	setupButtonsRelays();
	//tSensores.setInterval(5000, TSensoresLentos);
}
void TSensoresLentos()
{
  unsigned long currentMillis = millis();
   if(currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;   

	BlinkLedStatus = Send;
	sendMqttf("LastSeen", 1, false);
  }
	// reportIp();
	//ProcesarSensores();
	//InformarSensores();
	//	 Serial.println(analogRead(0));
	//	sendMqttf("FreeRam",freeMemory(),false);
}

void loop()
{

	OffLineMode = !ProcesarRed();
	if (OffLineMode != true) {
		CurentLedStatus = OffLine;
	}
	else
	{
		CurentLedStatus = Ok;
	}
	//ProcesarRed();
	DetectarBotones();
	ProcesarSensores();
//	ProcesarPushButtons();
	
	ProcesarWsStrip();
	TSensoresLentos();
}