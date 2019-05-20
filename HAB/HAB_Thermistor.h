#ifndef HAB_THERMISTOR_H
#define HAB_THERMISTOR_H

struct TemperatureData {
  float raw;
  float tempC;
  float tempF;
  };
  
class Thermistor {
  private:
  int m_address;

  public:
    Thermistor(int address);
    TemperatureData getTemperature();
};

#endif // HAB_SENSORS_H
