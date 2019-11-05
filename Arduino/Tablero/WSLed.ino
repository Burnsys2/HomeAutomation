#define FILLARRAY(a,n) a[0]=n, memcpy( ((char*)a)+sizeof(a[0]), a, sizeof(a)-sizeof(a[0]) );
const byte WSStripsSize = sizeof(WSStrips)/sizeof(WSStrips[0]);
eWsStripMode WsStripeMode[WSStripsSize];
int WsStripeParam1[WSStripsSize];
int WsStripeParam2[WSStripsSize];
CRGB WsStripeParamColor[WSStripsSize];
CRGB leds[WSStripsSize][100];
uint8_t gHue = 0; 
int Fps = 120;
unsigned long WsStartMilis;
unsigned long WsCurrentMilis;
byte WsDelayMilis = 100/Fps;

void SetupWsStrips()
{
	Serial.println(F("Cionfigurando WSStrips"));
	Serial.println(WSStripsSize);
// FastLED.addLeds<WS2812B, 9,BRG>(ledsbug, 1);
	for (byte index = 0; index < WSStripsSize; index++) {
        byte Pin =WSStrips[index][0];
        byte cntleds = WSStrips[index][1];
        Serial.print(F("WSStrip - Pin: "));
        Serial.print(Pin);
		Serial.print(F(" - Leds: "));
	       Serial.println(cntleds);
        WsStripeMode[index]  = AnimationStatic;
        switch (Pin)
        {
          //PIN 10 NO ANDA
   //    case 9:
 //           FastLED.addLeds<WS2812B, 9,BRG>(leds[index], cntleds);break;
       case 13:
            FastLED.addLeds<WS2812B, 13,GRB>(leds[index], cntleds);break;
        case 11:
            FastLED.addLeds<WS2812B, 11,BRG>(leds[index], cntleds);break;
		case 12:
            FastLED.addLeds<WS2812B, 12,BRG>(leds[index], cntleds);break;
       default:
            break;
        }
    }
  //  FastLED.show();

}

void ProcesarComandoWSLedsStrip(String topic, String valor)
{
    byte nro = getValue(topic,'/',4).toInt();
    String Mode = getValue(topic,'/',5);
    Mode.toUpperCase();
    WsStripeParam1[nro] = 0;
    WsStripeParam2[nro] = 0;


    if (Mode == F("FILL"))
    {
        WsStripeMode[nro] = AnimationStatic;
        CRGB Color = GetCrgbFromPayload(valor);
        //memset(leds[nro],Color,sizeof(leds[nro]));
        FILLARRAY(leds[nro],Color);
       // for (int NroLed = 0; NroLed < WSStrips[nro][1]; NroLed++) {
        //    leds[nro][NroLed] =  Color;
        //}
    }
    if (Mode == F("LED"))
    {
        WsStripeMode[nro] = AnimationStatic;
        int NroLed =  getValue(valor,',',0).toInt();
        leds[nro][NroLed] = GetCrgbFromPayload(valor);
    }
    if (Mode == F("FADETO"))  {
        WsStripeMode[nro] = AnimationFadeTo;
     
    }


    if (Mode == F("RAINBOW")) 
	{
		WsStripeParam1[nro] = gHue;
		WsStripeParam2[nro] = 7;

		WsStripeMode[nro] = AnimationRainbow;
		if (valor != "")
		{
			String hue = getValue(valor, ',', 0);
			if (hue != "")
			{
				WsStripeParam1[nro] = hue.toInt();
			}
			String delta = getValue(valor, ',', 1);
			if (delta != "")
			{
				WsStripeParam2[nro] = delta.toInt();
			}
		}
	}
    if (Mode == F("RAINBOWGLITTER"))  {WsStripeMode[nro] = AnimationRainbowWithGlitter;}
    if (Mode == F("CONFETTI"))  {WsStripeMode[nro] = AnimationConfetti;}
    if (Mode == F("SINELON"))  {
      WsStripeMode[nro] = AnimationSinelon;
      WsStripeParam1[nro] =  13;

      WsStripeParamColor[nro] = CHSV( gHue, 255, 192);
      if (valor != "")
      {
        WsStripeParamColor[nro] = GetCrgbFromPayload(valor);
        String bpm = getValue(valor,',',3);
        if (bpm != "")
        {
          WsStripeParam1[nro] =  bpm.toInt();
        }
      }
    }
    if (Mode == F("BPM"))  {WsStripeMode[nro] = AnimationBpm;}
    if (Mode == F("JUGGLE"))  {WsStripeMode[nro] = AnimationJuggle;}
  
    Serial.print(F("WSStrip Command: "));
    Serial.print(Mode);
    Serial.print(F(" - Strip: "));
    Serial.println(nro);
    
    FastLED.show();  
}

void ProcesarWsStrip()
{
  //return;
    WsCurrentMilis  = millis();
	if (WsCurrentMilis - WsStartMilis >= WsDelayMilis)  //test whether the period has elapsed
  	{
    	WsStartMilis = WsCurrentMilis;  //IMPORTANT to save the start time of the current LED brightness
        	for (byte nro = 0; nro < WSStripsSize; nro++) {
                if (WsStripeMode[nro] == AnimationRainbow)  {rainbow(nro);}
                if (WsStripeMode[nro] == AnimationRainbowWithGlitter)  {rainbowWithGlitter(nro);}
                if (WsStripeMode[nro] == AnimationConfetti)  {confetti(nro);}
                if (WsStripeMode[nro] == AnimationSinelon)  {sinelon(nro);}
                if (WsStripeMode[nro] == AnimationBpm)  {bpm(nro);}
                if (WsStripeMode[nro] == AnimationJuggle)  {juggle(nro);}
                if (WsStripeMode[nro] == AnimationFadeTo)  {fadeToColor(nro);}
                if (WsStripeMode[nro] != AnimationStatic)  {FastLED.show();}
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
  fadeToBlackBy( leds[index], WSStrips[index][1], 10);
  int pos = random16(WSStrips[index][1]);
  leds[index][pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon(byte index)
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds[index], WSStrips[index][1], 20);
  int pos = beatsin16(WsStripeParam1[index] , 0, WSStrips[index][1]-1 );
  leds[index][pos] += WsStripeParamColor[index];
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
  fadeToBlackBy( leds[index], WSStrips[index][1], 20);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[index][beatsin16( i+7, 0, WSStrips[index][1]-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}


// Fade an entire array of CRGBs toward a given background color by a given amount
// This function modifies the pixel array in place.
void fadeToColor(byte index)
{
    bool Done = true;

  fadeTowardColor( leds[index][0], WsStripeParamColor[index], WsStripeParam1[index]);
  FILLARRAY(leds[index],leds[index][0]);
  if (leds[index][0] != WsStripeParamColor[index])
  {
    Done = false;
  }

/*   for( uint16_t i = 0; i < WSStrips[index][1]; i++) {
    fadeTowardColor( leds[index][i], WsStripeFadeTo[index], WsStripeFadeToAmount[index]);
    if (leds[index][i] != WsStripeFadeTo[index])
    {
      Done = false;
    }
  }
  */
  if (Done) {  WsStripeMode[index] = AnimationStatic;}
  
}

void nblendU8TowardU8( uint8_t& cur, const uint8_t target, uint8_t amount)
{
  if( cur == target) return;
  
  if( cur < target ) {
    uint8_t delta = target - cur;
    delta = scale8_video( delta, amount);
    cur += delta;
  } else {
    uint8_t delta = cur - target;
    delta = scale8_video( delta, amount);
    cur -= delta;
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
