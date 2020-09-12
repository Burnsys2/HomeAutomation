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

        switch (Pin)
        {
        case 12:
            WScontrollers[index] = &FastLED.addLeds<WS2811, 12, BRG>(leds[index], cntleds); break;
        case 13:
            WScontrollers[index] = &FastLED.addLeds<WS2811, 13, BRG>(leds[index], cntleds); break;
       case 14:
            WScontrollers[index] = &FastLED.addLeds<WS2811, 14, BRG>(leds[index], cntleds); break;
        case 15:
            WScontrollers[index] = &FastLED.addLeds<WS2811, 15, BRG>(leds[index], cntleds); break;
    
        default:
            break;
        }
    }
    FastLED.show();

}
