const byte PushbuttonArraySize = sizeof(PushbuttonArray) / sizeof(PushbuttonArray[0]);
const byte PushbuttonAInvertedrraySize = sizeof(PushbuttonInvertedArray)/sizeof(PushbuttonInvertedArray[0]);

OneButton* buttons[PushbuttonArraySize];
int LongPressMilis = 1000;
int PressMilis= 200;
void setupPushButtons()
{
	for (byte index = 0; index < PushbuttonArraySize; index++) {
		byte currPin = PushbuttonArray[index];
		if (FindIndex(PushbuttonInvertedArray,PushbuttonArray[index],PushbuttonAInvertedrraySize) != -1)
		{
			buttons[index] = new OneButton(currPin,true);
		}
		else
		{
			buttons[index] = new OneButton(currPin,false);
		}
		buttons[index]->attachClick(PushButtonClickWrapper,&PushbuttonArray[index]);
		buttons[index]->attachDoubleClick(PushButtonDoubleClickWrapper,&PushbuttonArray[index]);
		buttons[index]->attachLongPressStart(PushButtonLongPressStartWrapper,&PushbuttonArray[index]);
		buttons[index]->attachLongPressStop(PushButtonLongPressStopWrapper,&PushbuttonArray[index]);
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
		buttons[index]->tick();
	}
}
void PushButtonClick(byte currPin) { sendMqttf(strButton + "/" + String(currPin), F("CLICK"), false); }
void PushButtonDoubleClick(byte currPin) {     sendMqttf(strButton + "/" + String(currPin), F("DOUBLECLICK"), false); }
void PushButtonDuringLongPress(byte currPin) { sendMqttf(strButton + "/" + String(currPin), F("DURINGLONGPRESS"), false); }
void PushButtonLongPressStart(byte currPin) { sendMqttf(strButton + "/" + String(currPin),F("LONGPRESSSTART"), false);}
void PushButtonLongPressStop(byte currPin)  { sendMqttf(strButton + "/" + String(currPin),F("LONGPRESSSTOP") , false);}

void PushButtonClickWrapper(void* arg) {
  byte currPin = *((byte*) arg);
  sendMqttf(strButton + "/" + String(currPin), F("CLICK"), false); 
 // PushButtonClick(currPin);
}

void PushButtonDoubleClickWrapper(void* arg) {
  byte  currPin = *((byte*) arg);
	PushButtonDoubleClick(currPin);
}
void PushButtonDuringLongPressWrapper(void* arg) {
  byte currPin = *((byte*) arg);
  PushButtonDuringLongPress(currPin);
}
void PushButtonLongPressStopWrapper(void* arg) {
  byte currPin = *((byte*) arg);
  PushButtonLongPressStop(currPin);
}
void PushButtonLongPressStartWrapper(void* arg) {
    byte currPin = *((byte*) arg);
	sendMqttf(strButton + "/" + String(currPin), F("LONGPRESSSTART"), false);
}

void ProcesarComandoButtons(String topic, String valor)
{
	if (getValue(topic, '/', 4) == F("PRESSMILIS"))
		PressMilis = valor.toInt();
	if (getValue(topic, '/', 4) == F("LONGPRESSMILIS"))
		LongPressMilis = valor.toInt();

	ConfigurePushButtons();
}

