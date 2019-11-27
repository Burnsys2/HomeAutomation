void SetupWsStrips()
{
    Serial.println(F("Cionfigurando WSStrips"));
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
            //PIN 10 NO ANDA
        case 3:
            FastLED.addLeds<WS2812B, 3, BRG>(leds[index], cntleds); break;
        case 4:
            FastLED.addLeds<WS2812B, 4, GRB>(leds[index], cntleds); break;

        default:
            break;
        }
    }
    FastLED.show();

}