byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x02 };
IPAddress ip(192, 168, 2, 42);
IPAddress mqtt_server(192, 168, 2, 1);
const String globalTopic = "Arduino";
const String sector = "PBTablero";

const byte IrRecPin = 0; //  CUALQUIER PIN;

//RF433  //		
//interrupts pins: 2, 3, 18, 19, 20, 21	
const byte rf433InPin = 21; //2+++++++++++++++++++++++++++++
const byte rf433OutPin = 20;
const byte rf315InPin = 0; //3
const byte rf315OutPin = 0;//7;

//RGB LEDS
const byte RGBLeds[][3] =
{
} ;
const byte EncodersPins[][2] =
{
	{25,23}
};
const byte WSLedStatus = 0;

//RGB LEDS
const byte WSStrips[][2] =
{
	  {11,14},
	  {12,57},   //techo
	  {13,11},   //central
	  {8,14},
	  {7,3}      //tablero
};

//PIN , TYPE (11 = DHT11, 22= DHT22)
const byte DHTArray[][2] =
{
     {6,22},
     {3,11}
};

//RGB LED STATUS
const byte LED_STATUS_RED_PIN = -1;
const byte LED_STATUS_GREEN_PIN = -1;
const byte LED_STATUS_BLUE_PIN = -1;
const byte LED_ACTION_RED_PIN =-1;
const byte LED_ACTION_GREEN_PIN =-1;
const byte LED_ACTION_BLUE_PIN = -1;

const byte analogInsArray[] = {3,5} ;
const byte analogInsOnlineArray[] = {};


const byte buttonArray[] = {49,47,45,43,41,39,37,35,33,31,29,27,17} ;
const byte PushbuttonArray[] = {31,37,27,29} ;
const byte RelaysArray[] = {22,24,26,28,30,32,34,36,38,40,42,44,46,48,50,52,4,5};
const byte RelaysInvertedArray[] = {40,5} ;
const byte RelaysManualMap[][2] =
{
     {33,34}, // VESTIDOR
     {43,30}, //escritorio
     {45,32}, // patio
     {49,28}, // Distribuidor
     {39,26}, //  comedor
     {41,48}, //  living
     {37,48}, //  living
     {35,34}, //  living
     {47,42} //  escalera
};
const byte buttonIgnoreNoiseFilterArray[] = {} ;
const int VoltSensorPin = 0;