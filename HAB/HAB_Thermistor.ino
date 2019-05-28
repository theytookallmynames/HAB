#include <math.h>
#include "HAB_Thermistor.h"
namespace HAB {
namespace Sensors {

// Some constants for the analog temperature conversion.
const float thermistorNominal = 10000.0;
const float temperatureNominal = 25.0;
const float bCoefficient = 3950.0;
const float pullupResistor = 10000.0;

// Constructor for the thermistor that accepts physical pin addresses
Thermistor::Thermistor(int pin) {
    m_pin = pin;
}

TemperatureData Thermistor::getTemperature() {
  TemperatureData data;
  float rawAverageVoltage = 0.0;
  for (uint8_t i = 0; i < 5; i++) {
    rawAverageVoltage += analogRead(m_pin);
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

#ifdef HAB_DEVELOPMENT
  Serial.println("PIN = " + String(m_pin));  
  Serial.println("RAW = " + String(data.raw));
  Serial.println("TEMP = " + String(data.tempC) + " C"); 
  Serial.println("TEMP = " + String(data.tempF) + " F");
  Serial.print("\n");
#endif
  return data;
}
}
}
