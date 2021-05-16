void SetupWsStrips()
{
    Serial.println(F("Cionfigurando WSStrips"));
    Serial.println(WSStripsSize);
    // FastLED.addLeds<WS2812B, 9,BRG>(ledsbug, 1);
    for (byte index = 0; index < WSStripsSize; index++) {
        byte Pin = WSStrips[index][0];
        byte cntleds = WSStrips[index][1];
        Serial.print(F("WSStrip - Pin: "));
        Serial.print(Pin);
        Serial.print(F(" - Leds: "));
        Serial.println(cntleds);
        WsStripeMode[index] = AnimationStatic;
        switch (Pin)
        { //PIN 10 NO ANDA
		case 7:
			WScontrollers[index] = &FastLED.addLeds<WS2812B, 7, BRG>(leds[index], cntleds); break;
		case 8:
			WScontrollers[index] = &FastLED.addLeds<WS2812B, 8, BRG>(leds[index], cntleds); break;
        case 13:
			WScontrollers[index] = &FastLED.addLeds<WS2812B, 13, BRG>(leds[index], cntleds); break;
        case 12:
			WScontrollers[index] = &FastLED.addLeds<WS2812B, 12, BRG>(leds[index], cntleds); break;
        case 11:
			WScontrollers[index] = &FastLED.addLeds<WS2812B, 11, BRG>(leds[index], cntleds); break;
        default:
            break;
        }	
    }
    //  FastLED.show();

}
