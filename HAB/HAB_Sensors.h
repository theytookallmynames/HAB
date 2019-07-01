#ifndef HAB_SENSORS_H
#define HAB_SENSORS_H
#include "HAB_Thermistor.h"

namespace HAB {
namespace Sensors {

Thermistor onboardThermistor = Thermistor(TEMPERATURE_ONBOARD_PIN);
Thermistor outdoorTopThermistor = Thermistor(TEMPERATURE_OUTDOOR_TOP_PIN);
Thermistor outdoorBottomThermistor = Thermistor(TEMPERATURE_OUTDOOR_BOTTOM_PIN);

/**
 * The I2C address for the pressure sensor. See Honeywell HSC PDF page 13 figure 4 (output type).
 */
#define PRESSURE_I2C_ADDR 0x28

/**
 * These values are used for converting the raw reading from the pressure sensor
 * to a usable value in units of bars.
 *
 * See page 3 of the Honeywell I2C communication PDF for the conversion formula.
 */
#define PRESSURE_MIN_OUTPUT 0.0 // Honeywell HSC PDF page 10 table 7
#define PRESSURE_MAX_OUTPUT 16383.0 // Honeywell HSC PDF page 10 table 7
#define PRESSURE_MIN_PRESSURE 0.0 // Honeywell HSC PDF page 13 figure 4
#define PRESSURE_MAX_PRESSURE 1.6 // Honeywell HSC PDF page 13 figure 4

struct PressureData {
  int raw;
  float bar;
};

bool init();
PressureData getPressureData();

} // namespace Sensors
} // namespace HAB

#endif // HAB_SENSORS_H
