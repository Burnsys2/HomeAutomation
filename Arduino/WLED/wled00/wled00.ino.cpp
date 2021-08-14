# 1 "C:\\Users\\Living\\AppData\\Local\\Temp\\tmp6f9ck1dy"
#include <Arduino.h>
# 1 "C:/Users/Living/source/repos/Burnsys2/HomeAutomation/Arduino/WLED/wled00/wled00.ino"
# 13 "C:/Users/Living/source/repos/Burnsys2/HomeAutomation/Arduino/WLED/wled00/wled00.ino"
#include "wled.h"
void setup();
void loop();
#line 15 "C:/Users/Living/source/repos/Burnsys2/HomeAutomation/Arduino/WLED/wled00/wled00.ino"
void setup() {
  WLED::instance().setup();
}

void loop() {
  WLED::instance().loop();
}