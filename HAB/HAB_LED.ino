#include "HAB_LED.h"
#include "HAB_Logging.h"

namespace HAB {
namespace LED {

bool init() {
  Logging::logSystemData("Initializing LED");
  strip.setBrightness(20);
  
  strip.begin();
  strip.show();

  strip.fill(success, 0, 7);
  statusLED(SYSTEM_STATUS_LED, success);
  strip.show();
  delay(1500);
  statusLED(SYSTEM_STATUS_LED, failure);
  strip.show();
  delay(1500);
  strip.clear();
  strip.show();

  Logging::logSystemData("LED initialized");
  return true;
}

void statusLED(int pin, Colors color) {
  strip.begin();
  strip.show();
  strip.setPixelColor(pin, color);
  strip.show();
}



} //namespace LED
} //namespace HAB
