unsigned long LedBlinkMillis;
eLedStatus LastStatus;


void SetupStatusLed()
{
	LastStatus = Ok;
	if (LED_STATUS_RED_PIN > 0)
	{
		pinMode(LED_STATUS_RED_PIN, OUTPUT);
	}
	if (LED_STATUS_BLUE_PIN > 0)
	{
		pinMode(LED_STATUS_BLUE_PIN, OUTPUT);
	}
	if (LED_STATUS_GREEN_PIN > 0)
	{
		pinMode(LED_STATUS_GREEN_PIN, OUTPUT);
	}

	if (LED_ACTION_RED_PIN > 0)
	{
		pinMode(LED_ACTION_RED_PIN, OUTPUT);
	}
	if (LED_ACTION_GREEN_PIN > 0)
	{
		pinMode(LED_ACTION_GREEN_PIN, OUTPUT);
	}
	if (LED_ACTION_BLUE_PIN > 0)
	{
		pinMode(LED_ACTION_BLUE_PIN, OUTPUT);
	}
	//SetLedStatus();
	SetLedBlink(0, 0, 0);

}
void SetLedActionWS()
{
	//     leds[0][0] = CRGB(2,2,2);
}
bool ActionLedPrendido;
eLedStatus LastStatusBlink;
void SetLedAction()
{
	if (LED_ACTION_RED_PIN <= 0)
	{
		return;
	}

	if (!ActionLedPrendido && BlinkLedStatus != None)
	{
		ActionLedPrendido = true;
		LedBlinkMillis = millis();
		//    Serial.println(millis());
	}
	if (!ActionLedPrendido && BlinkLedStatus == None)
	{
		return;
	}

	if (millis() - LedBlinkMillis > 20)
	{
		BlinkLedStatus = None;
		LedBlinkMillis = millis();
		ActionLedPrendido = false;
	}
	switch (BlinkLedStatus) {
	case None:
		SetLedBlink(0, 0, 0);
		break;
	case ReceiveAction:
		//  Serial.println("O");
		SetLedBlink(0, 1, 0);
		break;
	case Send:
		//  Serial.println("OF");
		SetLedBlink(1, 0, 0);
		break;
	case ManualAction:
		//  Serial.println("OF");
		SetLedBlink(0, 1, 1);
		break;
	case IrDetected:
		//  Serial.println("OF");
		SetLedBlink(1, 0, 1);
		break;
	case RFDetected:
		//  Serial.println("OF");
		SetLedBlink(0, 0, 1);
		break;
	case ButtonDetected:
		//  Serial.println("OF");
		SetLedBlink(0, 1, 1);
		break;
	}
}

void SetLedStatus()
{

	if (LastStatus == CurentLedStatus)
	{
		return;
	}
	LastStatus = CurentLedStatus;
	CRGB Color;
	switch (LastStatus) {
	case Starting:
		Color = CRGB(255, 0, 255);
		break;
	case Ok:
		Color = CRGB(0, 255, 0);
		break;
	case OffLine:
		Color = CRGB(255, 0, 0);
		break;
	}
	if (LED_STATUS_RED_PIN > 0)
	{
		analogWrite(LED_STATUS_RED_PIN, 255 - Color.red);
		analogWrite(LED_STATUS_GREEN_PIN, 255 - Color.green);
		analogWrite(LED_STATUS_BLUE_PIN, 255 - Color.blue);
	}

	if (WSLedStatus >= 0)
	{
		leds[WSLedStatus][0] = Color;
		FastLED.show();
	}

}

void SetLedBlink(bool red, bool green, bool blue)
{
	if (red)
	{
		digitalWrite(LED_ACTION_RED_PIN, LOW);
	}
	else
	{
		digitalWrite(LED_ACTION_RED_PIN, HIGH);
	}

	if (green)
	{
		digitalWrite(LED_ACTION_GREEN_PIN, LOW);
	}
	else
	{
		digitalWrite(LED_ACTION_GREEN_PIN, HIGH);
	}

	if (blue)
	{
		digitalWrite(LED_ACTION_BLUE_PIN, LOW);
	}
	else
	{
		digitalWrite(LED_ACTION_BLUE_PIN, HIGH);
	}
}

