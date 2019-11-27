/*
 Name:		Test_Arduino.ino
 Created:	12/25/2018 6:19:05 PM
 Author:	LIVING
*/
//#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <Arduino.h>
#define FASTLED_ESP8266_NODEMCU_PIN_ORDER
//#define FASTLED_ESP8266_D1_PIN_ORDER
#include "FastLED.h"
FASTLED_USING_NAMESPACE
#include <IRremoteESP8266.h>
#include <IRsend.h>
const uint16_t kIrLed = 4;
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
#include <DHT.h>
#include <SimpleTimer.h>
#include <ArduinoOTA.h>

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
	AnimationBpm,
	AnimationJuggle,
	AnimationFadeTo
};

eLedStatus CurentLedStatus;
eLedAction BlinkLedStatus;
bool OffLineMode = true;

//TIMERS
SimpleTimer tSensores;

void(*resetFunc) (void) = 0; //declare reset function @ address 0

void setup() {
	CurentLedStatus = Starting;
		//SetupStatusLed();
	//	SetLedStatus();
	Serial.begin(115200);
	while (!Serial) {}
	SetupWsStrips();
	setupEthernet();
	//pinMode(6, OUTPUT);
	//digitalWrite(6, HIGH);

	tSensores.setInterval(5000, TSensoresLentos);
}
void TSensoresLentos()
{
	sendMqttf("LastSeen", 1, false);
	// reportIp();
	BlinkLedStatus = Send;

	//	sendMqttf("FreeRam",freeMemory(),false);
}

void loop() {

	OffLineMode = !ProcesarRed();
//	digitalWrite(6, LOW);
	if (OffLineMode) {
		CurentLedStatus = OffLine;
	}
	else
	{
		CurentLedStatus = Ok;
	}
	ProcesarWsStrip();
	tSensores.run();
}