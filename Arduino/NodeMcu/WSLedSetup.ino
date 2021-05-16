void SetupWsStrips()
{
    Serial.println(F("Configurando WSStrips"));
    // FastLED.addLeds<WS2812B, 9,BRG>(ledsbug, 1);
    for (byte index = 0; index < WSStripsSize; index++) {
        byte Pin = WSStrips[index][0];
        int cntleds = WSStrips[index][1];
        Serial.print(F("WSStrip - Pin: "));
        Serial.print(Pin);
        Serial.print(F(" - Leds: "));
        Serial.println(cntleds);
        WsStripeMode[index] = AnimationStatic;
        WsStripeBrightness[index] = 255;

        switch (Pin)
        {  
        case 0:
            WScontrollers[index] = &FastLED.addLeds<WS2812B, 0, GRB>(leds[index], cntleds); break;
        case 2:
            WScontrollers[index] = &FastLED.addLeds<WS2812B, 2, GRB>(leds[index], cntleds); break;
        case 3:
            WScontrollers[index] = &FastLED.addLeds<WS2812B, 3, GRB>(leds[index], cntleds); break;
        case 12:
            WScontrollers[index] = &FastLED.addLeds<WS2811, 12, GRB>(leds[index], cntleds); break;
        case 13:
            WScontrollers[index] = &FastLED.addLeds<WS2811, 13, GRB>(leds[index], cntleds); break;
       case 14:
            WScontrollers[index] = &FastLED.addLeds<WS2812B, 14, GRB>(leds[index], cntleds); break;
        case 15:
            WScontrollers[index] = &FastLED.addLeds<WS2811, 15, GRB>(leds[index], cntleds); break;
    
        default:
            break;
        }
    }
    FastLED.show();

}
