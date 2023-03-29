//IPAddress ip(192, 168, 2, 41);
const String globalTopic = "NodeMcu";
const String sector = "Cocina";
const char* ssid = "Casa"; // Rellena con el nombre de tu red WiFi
const char* password = "casanueva"; // Rellena con la contrase�a de tu red WiFi
const char* mqtt_server = "192.168.2.1";

const bool BMP280Present = false;
const byte IrRecPin = 0;//2;

//RF433  //		
//interrupts pins: 2, 3, 18, 19, 20, 21	
const int Serialbps = 115200;
const byte rf433InPin = -1; //2
const byte rf433OutPin = -1;
const byte rf315InPin = -1; //3
const byte rf315OutPin = -1;//7;
const byte buttonArray[] = {};
const byte RelaysArray[] = {2};
const byte RelaysInvertedArray[] = {};
const byte RelaysManualMap[][2] =
{
};
const byte buttonIgnoreNoiseFilterArray[] = {};
//RGB LEDS
const byte RGBLeds[][3] =
{
};

//ws LEDS: Pins, CntLeds MISMO PIN QUE EL INDICADO
const byte WSStrips[][2] =
{
    {5,5} //57
};

const byte EncodersPins[][3] =
{
	{12,14,13}
};
const byte analogInsArray[] = {};
const byte analogInsOnlineArray[] = {};
const byte PushbuttonArray[] = {13,3,4,16} ;
const byte PushbuttonInvertedArray[] = {13} ;
// D0 = 16; Tecla 1
// D1 = 5;  Leds
// D2 = 4;  Tecla 2
// D3 = 0;  
// D4 = 2;  Relay 1
// D5 = 14; ENCODER 
// D6 = 12; ENCODER
// D7 = 13; BOTON ENCODER
// D8 = 15;
// D9 = 3;  Tecla 3
// D10 = 1;
//