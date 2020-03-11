#define FILLARRAY(a,n) a[0]=n, memcpy( ((char*)a)+sizeof(a[0]), a, sizeof(a)-sizeof(a[0]) );
const byte WSStripsSize = sizeof(WSStrips)/sizeof(WSStrips[0]);
eWsStripMode WsStripeMode[WSStripsSize];
CLEDController *WScontrollers[WSStripsSize];
int WsStripeParam1[WSStripsSize];
int WsStripeParam2[WSStripsSize];
int WsStripeAux1[WSStripsSize];

long WsStripeMillis[WSStripsSize];
CRGB WsStripeParamColor[WSStripsSize];
CRGB leds[WSStripsSize][100];
uint8_t gHue = 0; 
int Fps = 120;
unsigned long WsStartMilis;
unsigned long WsCurrentMilis;
int WsDelayMilis = 1000;

void ProcesarComandoWSLedsStrip(String topic, String valor)
{
	String param = getValue(topic, '/', 4);
	param.toUpperCase();
	if (param == "FPS")
	{
		Fps = getValue(valor, ',', 0).toInt();
		WsDelayMilis = 1000 / Fps;
		return;
	}


	//	byte nro = getValue(topic,'/',4).toInt();
	char NrosLeds[25];
	getValue(topic, '/', 4).toCharArray(NrosLeds, 25);
	byte cnt = 0;
	char* ptr = strtok(NrosLeds, ",");

	while (ptr != NULL) {
		unsigned int nro = atoi(ptr);

		Serial.print(F("Strip: "));
		Serial.print(nro);

		String Mode = getValue(topic,'/',5);
		Mode.toUpperCase();
		if (Mode == F("REFRESH"))
		{
			WScontrollers[nro]->showLeds();
			return;
		}
		WsStripeParam1[nro] = 0;
		WsStripeParam2[nro] = 0;

	//	Serial.print(F(" - WSStrip Command: "));
		//Serial.println(Mode);
		if (Mode == F("FILL"))
		{
			WsStripeMode[nro] = AnimationStatic;
			CRGB Color = GetCrgbFromPayload(valor);
			fill_solid(leds[nro], WSStrips[nro][1], Color);
		//	FILLARRAY(leds[nro],Color);
		}
		if (Mode == F("LED"))
		{
			WsStripeMode[nro] = AnimationStatic;
			int NroLed =  getValue(valor,',',0).toInt();
			leds[nro][NroLed] = GetCrgbFromPayload(valor);
		}
		if (Mode == F("FADETO"))  
		{
			if (valor == "") return;
			WsStripeMode[nro] = AnimationFadeTo;     
			WsStripeParam1[nro] = 13; //speed
			WsStripeParamColor[nro] = GetCrgbFromPayload(valor);
			String speed = getValue(valor, ',', 3);
			if (speed != "") WsStripeParam1[nro] = speed.toInt();
		}
		if (Mode == F("HUESWIPE"))  
		{
			if (valor == "") return;
			WsStripeMode[nro] = AnimationHueSwipe;     
			WsStripeParam1[nro] = 13; //speed
			WsStripeParam2[nro] = 255; //brillo
			WsStripeAux1[nro] = gHue; // colorInicial
			String speed = getValue(valor, ',', 0);
			if (speed != "") WsStripeParam1[nro] = speed.toInt();
		}
		if (Mode == F("RAINBOW")) 
		{
			WsStripeParam2[nro] = 7;
			WsStripeAux1[nro] = gHue; // colorInicial

			WsStripeMode[nro] = AnimationRainbow;
			String delta = getValue(valor, ',', 0);
			if (delta != "") WsStripeParam2[nro] = delta.toInt();
			rainbow(nro);
		}
			if (Mode == F("RAINBOWSPIN")) 
		{
			WsStripeParam2[nro] = 7; //speed
			WsStripeAux1[nro] = gHue;
			WsStripeMode[nro] = AnimationRainbowSpin;
			String hue = getValue(valor, ',', 0);
			if (hue != "") WsStripeParam1[nro] = hue.toInt();
			String delta = getValue(valor, ',', 1);
			if (delta != "") WsStripeParam2[nro] = delta.toInt();
			rainbow(nro);
		}
		
		if (Mode == F("RAINBOWGLITTER"))  WsStripeMode[nro] = AnimationRainbowWithGlitter;
		if (Mode == F("CONFETTI"))
		{
			WsStripeParam1[nro] = 10;
			WsStripeParam2[nro] = 80; //chances de que aparezca
			WsStripeMode[nro] = AnimationConfetti;
			String speed = getValue(valor, ',', 0);
			if (speed != "")	WsStripeParam1[nro] = speed.toInt();
			String chances = getValue(valor, ',', 1);
			if (chances != "")	WsStripeParam2[nro] = chances.toInt();
		}
		if (Mode == F("SINELON"))  {
		  WsStripeMode[nro] = AnimationSinelon;
		  WsStripeParam1[nro] =  13;

		  WsStripeParamColor[nro] = CHSV( gHue, 255, 192);
		  if (valor != "")
		  {
			WsStripeParamColor[nro] = GetCrgbFromPayload(valor);
			String bpm = getValue(valor,',',3);
			if (bpm != "") WsStripeParam1[nro] =  bpm.toInt();
		  }
		}
		if (Mode == F("STROBE")) {
			WsStripeMode[nro] = AnimationStrobe;
			WsStripeParam1[nro] = 13; // velocidad
			WsStripeParam2[nro] = 0; //estado
			WsStripeParamColor[nro] = CRGB(255, 255, 255);
			if (valor != "")
			{
				WsStripeParamColor[nro] = GetCrgbFromPayload(valor);
				String bpm = getValue(valor, ',', 3);
				if (bpm != "") WsStripeParam1[nro] = bpm.toInt();
			}
		}
		if (Mode == F("BPM"))  WsStripeMode[nro] = AnimationBpm;
		if (Mode == F("JUGGLE"))

		{
			WsStripeMode[nro] = AnimationJuggle;
			String speed = getValue(valor, ',', 0);
			if (speed != "")	WsStripeParam1[nro] = speed.toInt();

		}
		WScontrollers[nro]->showLeds();
		ptr = strtok(NULL, ",");
	}

   // FastLED.show();  
}

void ProcesarWsStrip()
{
  //return;
    WsCurrentMilis  = millis();
	if (WsCurrentMilis - WsStartMilis >= WsDelayMilis)  //test whether the period has elapsed
  	{
    	WsStartMilis = WsCurrentMilis;  //IMPORTANT to save the start time of the current LED brightness
        	for (byte nro = 0; nro < WSStripsSize; nro++) {
           //     if (WsStripeMode[nro] == AnimationRainbow)  {rainbow(nro);}
                if (WsStripeMode[nro] == AnimationRainbowWithGlitter)  {rainbowWithGlitter(nro);}
                if (WsStripeMode[nro] == AnimationConfetti)  {confetti(nro);}
				if (WsStripeMode[nro] == AnimationSinelon) { sinelon(nro); }
				if (WsStripeMode[nro] == AnimationStrobe)  { strobe(nro);}
                if (WsStripeMode[nro] == AnimationBpm)  {bpm(nro);}
                if (WsStripeMode[nro] == AnimationJuggle)  {juggle(nro);}
                if (WsStripeMode[nro] == AnimationFadeTo)  {fadeToColorf(nro);}
                if (WsStripeMode[nro] == AnimationHueSwipe)  {HueSwipe(nro);}
                if (WsStripeMode[nro] == AnimationRainbowSpin)  {rainbowSpin(nro);}
                if (WsStripeMode[nro] != AnimationStatic && WsStripeMode[nro] != AnimationRainbow && WsStripeMode[nro] != AnimationRainbowSpin && WsStripeMode[nro] != AnimationHueSwipe)  {WScontrollers[nro]->showLeds();}
            }
  	}
}

CRGB GetCrgbFromPayload(String payload)
{
    return CRGB( getValue(payload,',',0).toInt(), getValue(payload,',',1).toInt(), getValue(payload,',',2).toInt()); 
}

void rainbow(byte index) 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds[index], WSStrips[index][1], WsStripeParam1[index], WsStripeParam2[index]);
}

void rainbowWithGlitter(byte index) 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow(index);
  addGlitter(index, 80);
}

void addGlitter(byte index, fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[index][ random16(WSStrips[index][1]) ] += CRGB::White;
  }
}

void confetti(byte index) 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds[index], WSStrips[index][1], WsStripeParam1[index]);
  int pos = random16(WSStrips[index][1]);
  if (random8() < WsStripeParam2[index]) {
	leds[index][pos] += CHSV(random8(255), 200, 255);
	  
  }
}

void sinelon(byte index)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds[index], WSStrips[index][1], 20);
  int pos = beatsin16(WsStripeParam1[index] , 0, WSStrips[index][1]-1 );
  leds[index][pos] += WsStripeParamColor[index];
}

void strobe(byte index)
{
	if (millis() - WsStripeMillis[index] > WsStripeParam1[index])
	{
		WsStripeMillis[index] = millis();
		if (WsStripeParam2[index] == 0)
		{ 
			fill_solid(leds[index], WSStrips[index][1], WsStripeParamColor[index]);
			WsStripeParam2[index] = 1;
		}
		else
		{
			fill_solid(leds[index], WSStrips[index][1], CRGB::Black);
			WsStripeParam2[index] = 0;
		}
		WScontrollers[index]->showLeds();
	}
}

void bpm(byte index)
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( WsStripeParam1[index], 64, 255);
  for( int i = 0; i < WSStrips[index][1]; i++) { //9948
    leds[index][i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle(byte index) {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds[index], WSStrips[index][1], WsStripeParam1[index]);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[index][beatsin16( i+7, 0, WSStrips[index][1]-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


void fadeToColorf(byte index)
{
	bool Done = true;
	for (int i = 0; i < WSStrips[index][1]; i++)
	{
		if (leds[index][i] != WsStripeParamColor[index])
		{
			fadeTowardColor(leds[index][i], WsStripeParamColor[index], WsStripeParam1[index]);
			Done = false;
		}
	}
	if (Done)
	{
		WsStripeMode[index] = AnimationStatic;
		WScontrollers[index]->showLeds();
	}
}
// Fade an entire array of CRGBs toward a given background color by a given amount
// This function modifies the pixel array in place.
void HueSwipe(byte index)
{
	if (millis() - WsStripeMillis[index] > WsStripeParam1[index])
	{
		WsStripeMillis[index] = millis();
		WsStripeAux1[index] = WsStripeAux1[index] + 1;
		if (WsStripeAux1[index] > 256) WsStripeAux1[index] -=256;	
		fill_solid(leds[index], WSStrips[index][1], CHSV(WsStripeAux1[index], 255, WsStripeParam2[index]));
		WScontrollers[index]->showLeds();
	}
}
void rainbowSpin(byte index) 
{
  // FastLED's built-in rainbow generator
	if (millis() - WsStripeMillis[index] > WsStripeParam1[index])
	{
		WsStripeMillis[index] = millis();
		WsStripeAux1[index] = WsStripeAux1[index] + 1;
		if (WsStripeAux1[index] > 256) WsStripeAux1[index] -=256;	
	    fill_rainbow( leds[index], WSStrips[index][1], WsStripeAux1[index], WsStripeParam2[index]);
		WScontrollers[index]->showLeds();
	}
}
// Fade an entire array of CRGBs toward a given background color by a given amount
// This function modifies the pixel array in place.
void fadeToColor(byte index)
{
  bool Done = true;
  fadeTowardColor( leds[index][0], WsStripeParamColor[index], WsStripeParam1[index]);
  fill_solid(leds[index], WSStrips[index][1], leds[index][0]);
  //FILLARRAY(leds[index],leds[index][0]);
  if (leds[index][0] != WsStripeParamColor[index])
		Done = false;
  if (Done) 
  {  
	  WsStripeMode[index] = AnimationStatic;
	  WScontrollers[index]->showLeds();
  }
}
// Blend one CRGB color toward another CRGB color by a given amount.
// Blending is linear, and done in the RGB color space.
// This function modifies 'cur' in place.
CRGB fadeTowardColor( CRGB& cur, const CRGB& target, uint8_t amount)
{
  nblendU8TowardU8( cur.red,   target.red,   amount);
  nblendU8TowardU8( cur.green, target.green, amount);
  nblendU8TowardU8( cur.blue,  target.blue,  amount);
  return cur;
}

void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
  //255  , 10
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount * 2);
    cur -= delta;
  }
}

