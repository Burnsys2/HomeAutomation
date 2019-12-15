
//IPAddress ip(192, 168, 2, 41);
const String Mqtt = "192.168.2.1";
const String globalTopic = "NodeMcu";
const String sector = "NodeTest";
const char* ssid = "Casa"; // Rellena con el nombre de tu red WiFi
const char* password = "casanueva"; // Rellena con la contraseña de tu red WiFi
const char* mqtt_server = "192.168.2.1";


const byte IrRecPin = 9;//2;

//RF433  //		
//interrupts pins: 2, 3, 18, 19, 20, 21	
const byte rf433InPin = -1; //2
const byte rf433OutPin = -1;
const byte rf315InPin = -1
; //3
const byte rf315OutPin = -1;//7;
const byte buttonArray[] = {16};
const byte RelaysArray[] = {};
const byte RelaysInvertedArray[] = {};
const byte RelaysManualMap[][2] =
{
};
const byte buttonIgnoreNoiseFilterArray[] = {};
//RGB LEDS
const byte RGBLeds[][3] =
{
	 {2,4,6}
};

//ws LEDS: Pins, CntLeds MISMO PIN QUE EL INDICADO
const byte WSStrips[][2] =
{
	 {3,4}, //57
	 {4,15} //57
};


#define DHT_PIN 0
#define DHT_TYPE DHT22

// D0 = 16;
// D1 = 5;
// D2 = 4;
// D3 = 0;
// D4 = 2;
// D5 = 14;
// D6 = 12;
// D7 = 13;
// D8 = 15;
// D9 = 3;
// D10 = 1;
//