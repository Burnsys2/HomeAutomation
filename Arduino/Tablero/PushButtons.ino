//botones
const byte PushbuttonArraySize = sizeof(PushbuttonArray) / sizeof(PushbuttonArray[0]);
OneButton* buttons[PushbuttonArraySize];
int LongPressMilis = 1000;
int PressMilis= 200;
byte currentPushButton = 0;
void setupPushButtons()
{
	for (byte index = 0; index < PushbuttonArraySize; index++) {
		buttons[index] = new OneButton(PushbuttonArray[index],false);
		buttons[index]->attachClick(PushButtonClick);
		buttons[index]->attachDoubleClick(PushButtonDoubleClick);
		buttons[index]->attachDuringLongPress(PushButtonDuringLongPress);
		buttons[index]->attachLongPressStart(PushButtonLongPressStart);
		buttons[index]->attachLongPressStop(PushButtonLongPressStop);
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
		currentPushButton = PushbuttonArray[index];
		buttons[index]->tick();
	}
}
void PushButtonClick() { sendMqttf("Button/" + String(currentPushButton), "CLICK", false); }
void PushButtonDoubleClick() { sendMqttf("Button/" + String(currentPushButton), "DOUBLECLICK", false); }
void PushButtonDuringLongPress() { sendMqttf("Button/" + String(currentPushButton), "DURINGLONGPRESS", false); }
void PushButtonLongPressStart() { sendMqttf("Button/" + String(currentPushButton), "LONGPRESSSTART", false); }
void PushButtonLongPressStop(){sendMqttf("Button/" + String(currentPushButton),"LONGPRESSSTOP", false);}


void ProcesarComandoButtons(String topic, String valor)
{
	if (getValue(topic, '/', 4) == F("PRESSMILIS"))
		PressMilis = valor.toInt();
	if (getValue(topic, '/', 4) == F("LONGPRESSMILIS"))
		LongPressMilis = valor.toInt();

	ConfigurePushButtons();
}


