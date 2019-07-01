#include <Wire.h>
#include <math.h>
#include "HAB.h"
#include "HAB_Sensors.h"
#include "HAB_Thermistor.h"
#include "HAB_Logging.h"
#include "HAB_LED.h"

namespace HAB {
namespace Sensors {

bool initPressureSensors();
bool initThermistors();

bool init() {
 Logging::logSystemData("Initializing sensors now...");
 bool success = initPressureSensors() && initThermistors();
 return success;
}

/**
 * Initialize the pressure sensors.
 */
bool initPressureSensors() {
  Logging::logSystemData("Initializing Pressure Sensors...");
  LED::statusLED(PRESSURE_SENSOR_STATUS_LED, LED::pending);
  Wire.begin();
  delay(500);

  PressureData initialPressure = getPressureData();
  if (initialPressure.bar <= 0.0) {
    Logging::logSystemData(
      "Pressure sensor initialization failed: got pressure lower than 0 (" +
      String(initialPressure.bar) +
      " bar)"
    );
    LED::statusLED(PRESSURE_SENSOR_STATUS_LED, LED::failure);
    return false;
  }

  Logging::logSystemData(
    "Pressure sensors initialized. Initial pressure: " +
    String(initialPressure.bar) +
    " bar"
  );
  LED::statusLED(PRESSURE_SENSOR_STATUS_LED, LED::success);

  return true;
}

/**
 * Temperature initialization consists of
 */
bool initThermistors() {
  LED::statusLED(ONBOARD_TEMPERATURE_STATUS_LED, LED::pending);
  LED::statusLED(OUTDOOR_TOP_TEMPERATURE_STATUS_LED, LED::pending);
  LED::statusLED(OUTDOOR_BOTTOM_TEMPERATURE_STATUS_LED, LED::pending);

  TemperatureData onboardTemperature = onboardThermistor.getTemperature();
  TemperatureData outdoorTopTemperature = outdoorTopThermistor.getTemperature();
  TemperatureData outdoorBottomTemperature = outdoorBottomThermistor.getTemperature();

  if (onboardTemperature.tempC <= 0.0) {
    Logging::logSystemData(
      "Onboard temperature sensor initialization failed: got temperature lower than 0 (" +
      String(onboardTemperature.tempC) +
      " C)"
    );

    LED::statusLED(ONBOARD_TEMPERATURE_STATUS_LED, LED::failure);
    return false;
  } else {
    Logging::logSystemData(
      "Onboard temperature sensor initialized. Initial temperature: " +
      String(onboardTemperature.tempC) +
      "C"
    );
    LED::statusLED(ONBOARD_TEMPERATURE_STATUS_LED, LED::success);
  }

  if (outdoorTopTemperature.tempC <= 0.0) {
    Logging::logSystemData(
      "Outdoor top temperature sensor initialization failed: got temperature lower than 0 (" +
      String(outdoorTopTemperature.tempC) +
      " C)");

     LED::statusLED(OUTDOOR_TOP_TEMPERATURE_STATUS_LED, LED::failure);
     return false;
  } else {
    Logging::logSystemData(
      "Outdoor top temperature sensor initialized. Initial temperature: " +
      String(outdoorTopTemperature.tempC) +
      "C"
    );
    LED::statusLED(OUTDOOR_TOP_TEMPERATURE_STATUS_LED, LED::success);
  }

  if (outdoorBottomTemperature.tempC <= 0.0) {
    Logging::logSystemData(
      "Outdoor bottom temperature sensor initialization failed: got temperature lower than 0 (" +
      String(outdoorBottomTemperature.tempC) +
      " C)"
    );
    LED::statusLED(OUTDOOR_BOTTOM_TEMPERATURE_STATUS_LED, LED::failure);
    return false;
  } else {
    Logging::logSystemData(
      "Outdoor bottom temperature sensor initialized. Initial temperature: " +
      String(outdoorBottomTemperature.tempC) +
      "C"
    );
    LED::statusLED(OUTDOOR_BOTTOM_TEMPERATURE_STATUS_LED, LED::success);
  }

  return true;
}
/**
 * Convert the raw reading from the pressure sensor to a usable pressure in units of bars.
 * See page 2 of the Honeywell I2C Communication PDF for details on the data protocol,
 * and page 3 for the conversion formula.
 */
float rawPressureToBar(int raw) {
  float pressure = ((float)raw - PRESSURE_MIN_OUTPUT) * (PRESSURE_MAX_PRESSURE - PRESSURE_MIN_PRESSURE);
  pressure /= (PRESSURE_MAX_OUTPUT - PRESSURE_MIN_OUTPUT);
  pressure += PRESSURE_MIN_PRESSURE;

  return pressure;
}

/**
 * Get a reading from the pressure sensors. Returns a PressureData struct containing the
 * raw sensor data and the pressure in bar units.
 */
PressureData getPressureData() {
  PressureData data;

  // I'm not sure this is necessary, but it's in one of the Wire examples:
  // https://www.arduino.cc/en/Tutorial/SFRRangerReader
  Wire.beginTransmission(PRESSURE_I2C_ADDR);
  Wire.write(0);
  Wire.endTransmission();

  // Request two bytes from the pressure sensor
  Wire.requestFrom(PRESSURE_I2C_ADDR, 2);

  // Wait until two bytes are available from the sensor
  while (Wire.available() < 2);
  byte firstByte = Wire.read();
  byte secondByte = Wire.read();

  // https://github.com/AlexSatrapa/SSC/blob/819f06b4f4ce40cb438103ef416f8990c1536ea6/SSC.cpp#L37
  data.raw = (((int)(firstByte & 0x3f)) << 8) | secondByte;
  data.bar = rawPressureToBar(data.raw);

  return data;
}

} // namespace Sensors
} // namespace HAB
