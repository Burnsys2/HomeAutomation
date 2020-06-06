SDL_Arduino_INA3221 ina3221;
int MID;
void SetupINA()
{
   
  ina3221.begin();
  MID = ina3221.getManufID();
  Serial.println("IN:" + String(MID));

}
void ProcesarINA()
{
   if (MID <= -0) return;
	for (byte index = 1; index < 4; index++) {
		sendMqttfPrec(strSensores + "/INA/" + String(index) + "/VOLTS", ina3221.getBusVoltage_V(index), false);
		sendMqttfPrec(strSensores + "/INA/" + String(index) + "/SHUNTMV", ina3221.getShuntVoltage_mV(index), false);
		sendMqttfPrec(strSensores + "/INA/" + String(index) + "/CURRENT", ina3221.getCurrent_mA(index), false);
    }
}

