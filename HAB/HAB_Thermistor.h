#ifndef HAB_THERMISTOR_H
#define HAB_THERMISTOR_H

namespace HAB {
namespace Sensors {

struct TemperatureData {
  float raw;
  float tempC;
  float tempF;
  };
  
class Thermistor {
  private:
  int m_pin;

  public:
    Thermistor(int pin);
    TemperatureData getTemperature();
};

}
}
#endif // HAB_SENSORS_H
