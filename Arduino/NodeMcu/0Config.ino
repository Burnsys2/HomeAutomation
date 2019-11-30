
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

//RGB LEDS
const byte RGBLeds[][3] =
{
	 {2,4,6}
};

//ws LEDS: Pins, CntLeds MISMO PIN QUE EL INDICADO
const byte WSStrips[][2] =
{
	 {3,57}
};


#define DHT_PIN 0
#define DHT_TYPE DHT22
//LUZ
//RGB LED STATUS
const byte LED_STATUS_RED_PIN = 0;
const byte LED_STATUS_GREEN_PIN = 0;
const byte LED_STATUS_BLUE_PIN = 0;

const byte LED_ACTION_RED_PIN = 0;
const byte LED_ACTION_GREEN_PIN = 0;
const byte LED_ACTION_BLUE_PIN = 0;
