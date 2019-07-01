#ifndef HAB_LED_H
#define HAB_LED_H
#include <Adafruit_NeoPixel.h>

namespace HAB {
namespace LED {

#define LED_COUNT 8

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN);

// The numbers refer to the respective LED on the neopixel strip
#define GPS_STATUS_LED 0
#define SD_STATUS_LED 1
#define PRESSURE_SENSOR_STATUS_LED 2
#define ONBOARD_TEMPERATURE_STATUS_LED 3
#define OUTDOOR_TOP_TEMPERATURE_STATUS_LED 4
#define OUTDOOR_BOTTOM_TEMPERATURE_STATUS_LED 5
#define SYSTEM_STATUS_LED 7

/**
 * The values for the Colors enum were determined by printing the uint32_t value used to
 * set the color for the lights to the serial monitor.  We then put the int value
 * printed out in the enum.
 *
 * For example, success is green so the value was uint32_t success = strip.Color(0, 255, 0).
 * When success is printed to the serial monitor, the value 65280 is given so that is the enum value.
 */
enum Colors {
  success = 65280,
  failure = 16711680,
  pending = 16776960
};

bool init();
void statusLED(int, Colors);

} // namespace LED
} // namespace HAB

#endif //HAB_LED_H
