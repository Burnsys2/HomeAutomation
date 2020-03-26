const byte PushbuttonArraySize = sizeof(PushbuttonArray) / sizeof(PushbuttonArray[0]);
OneButton* buttons[PushbuttonArraySize];
int LongPressMilis = 1000;
int PressMilis= 200;
//byte currentPushButton = 0;
void setupPushButtons()
{
	for (byte index = 0; index < PushbuttonArraySize; index++) {
		byte currPin = PushbuttonArray[index];
		buttons[index] = new OneButton(currPin,false);
		buttons[index]->attachClick(PushButtonClick,currPin);
		buttons[index]->attachDoubleClick(PushButtonDoubleClick,currPin);
	//	buttons[index]->attachDuringLongPress(PushButtonDuringLongPress);
		buttons[index]->attachLongPressStart(PushButtonLongPressStart,currPin);
		buttons[index]->attachLongPressStop(PushButtonLongPressStop,currPin);
	}
	ConfigurePushButtons();
}

void ConfigurePushButtons()
{
	for (byte index = 0; index < PushbuttonArraySize; index++) {
		buttons[index]->setDebounceTicks(noiseMillis);
		buttons[index]->setClickTicks(PressMilis);
		buttons[index]->setPressTicks(LongPressMilis);
	}
}
void ProcesarPushButtons()
{
	for (byte index = 0; index < PushbuttonArraySize; index++) {
	//	currentPushButton = PushbuttonArray[index];
		buttons[index]->tick();
	}
}
void PushButtonClick(byte currPin) { sendMqttf("Button/" + String(currPin), "CLICK", false); }
void PushButtonDoubleClick(byte currPin) { sendMqttf("Button/" + String(currPin), "DOUBLECLICK", false); }
void PushButtonDuringLongPress(byte currPin) { sendMqttf("Button/" + String(currPin), "DURINGLONGPRESS", false); }
void PushButtonLongPressStart(byte currPin) { sendMqttf("Button/" + String(currPin), "LONGPRESSSTART", false); }
void PushButtonLongPressStop(byte currPin){sendMqttf("Button/" + String(currPin),"LONGPRESSSTOP", false);}


void ProcesarComandoButtons(String topic, String valor)
{
	if (getValue(topic, '/', 4) == F("PRESSMILIS"))
		PressMilis = valor.toInt();
	if (getValue(topic, '/', 4) == F("LONGPRESSMILIS"))
		LongPressMilis = valor.toInt();

	ConfigurePushButtons();
}

