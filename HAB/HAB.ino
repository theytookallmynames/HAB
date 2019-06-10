#include "HAB.h"
#include "HAB_GPS.h"
#include "HAB_Logging.h"
#include "HAB_Sensors.h"
#include "HAB_Thermistor.h"
#include "HAB_Door.h"
using namespace HAB;


void setup() {
  Serial.begin(9600);

// Block until the serial monitor is connected in development
#ifdef HAB_DEVELOPMENT
  while(!Serial);
#endif

 Logging::logSystemData("System startup");

 if (!GPS::init() || !Logging::init() || !Sensors::init()) {
   Logging::logSystemData("A subsystem failed to initialize properly. Aborting.");
   return;
 }

 Logging::logSystemData("All subsystems initialized, starting main loop.");
}

// TODO: actual logic
void loop() {
  Door::DoorStatus door1Status = Door::getDoorStatus1();
  Door::DoorStatus door2Status = Door::getDoorStatus2();
  GPS::GPSTimeData gpsTime = GPS::getCurrentUtcTime();
  const char* rawGpsData = GPS::getRawGPSData();
  long gpsAltitude = GPS::getCurrentAltitude();
  Sensors::TemperatureData onboardTemperature = Sensors::onboardThermistor.getTemperature();
  Sensors::TemperatureData outdoorTopTemperature = Sensors::outdoorTopThermistor.getTemperature();
  Sensors::TemperatureData outdoorBottomTemperature = Sensors::outdoorBottomThermistor.getTemperature();
  Sensors::PressureData pressure = Sensors::getPressureData();
  // Sensors::PressureData pressure;
  // pressure.raw = 2;
  // pressure.bar = 4;

  Logging::MissionData data = Logging::MissionData(rawGpsData, gpsAltitude, onboardTemperature, outdoorTopTemperature, outdoorBottomTemperature, pressure, door1Status, door1Status);
  Logging::logMissionData(data.toString());
  adjustDoorIfNeeded(gpsAltitude);
  

 if (GPS::process()) {
   Logging::logSystemData(GPS::getRawGPSData());
   Logging::logSystemData(String(GPS::getCurrentAltitude()));
 }
   delay(1000);
}
