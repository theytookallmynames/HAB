#include <Wire.h>
#include <math.h>
#include "HAB.h"
#include "HAB_Sensors.h"
#include "HAB_Logging.h"

namespace HAB {
namespace Sensors {

bool initPressureSensors();
bool initTempSensors();

bool init() {
  pinMode(SENSOR_STATUS_LED_PIN, OUTPUT);
  bool success = initTempSensors() && initPressureSensors();
  if (success) {
    digitalWrite(SENSOR_STATUS_LED_PIN, HIGH);
  }
  return success;
}

bool initTempSensors() {
  pinMode(TEMPERATURE_ONBOARD_PIN, INPUT);
  Logging::logSystemData("Temperature sensors initialized");
  return true;
}

/**
 * Initialize the pressure sensors.
 */
bool initPressureSensors() {
  Wire.begin();
  delay(500);

  PressureData initialPressure = getPressureData();
  if (initialPressure.bar <= 0.0) {
    Logging::logSystemData(
      "Pressure sensor initialization failed: got pressure lower than 0 (" +
      String(initialPressure.bar) +
      " bar)"
    );
    return false;
  }

  Logging::logSystemData(
    "Pressure sensors initialized. Initial pressure: " +
    String(initialPressure.bar) +
    " bar"
  );

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

// Some constants for the analog temperature conversion.
const float thermistorNominal = 10000.0;
const float temperatureNominal = 25.0;
const float bCoefficient = 3950.0;
const float pullupResistor = 10000.0;

/**
 * Get a reading from the analog onboard temperature sensor.
 * TODO: we're probably only going to be using analog since we couldn't get
 * the digital sensors working.
 *
 * See https://learn.adafruit.com/thermistor/using-a-thermistor for more info.
 */
TemperatureData getOnboardTemperature() {
  TemperatureData data;
  float rawAverageVoltage = 0.0;

  for (uint8_t i = 0; i < 5; i++) {
    rawAverageVoltage += analogRead(TEMPERATURE_ONBOARD_PIN);
    delay(10);
  }

  rawAverageVoltage /= 5.0;
  data.raw = rawAverageVoltage;

  float rawResistance = pullupResistor / (1023 / rawAverageVoltage - 1);

  float steinhart = rawResistance / thermistorNominal;
  steinhart = log(steinhart) / bCoefficient;
  steinhart += 1.0 / (temperatureNominal + 273.15);
  steinhart = (1.0 / steinhart) - 273.15;

  data.tempC = steinhart;
  data.tempF = (data.tempC * 1.8) + 32.0;

  return data;
}

} // namespace Sensors
} // namespace HAB

