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

 if (!GPS::init() || !Logging::init()|| !Sensors::init()) {
   Logging::logSystemData("A subsystem failed to initialize properly. Aborting.");
   return;
 }

 Logging::logSystemData("All subsystems initialized, starting main loop.");
}

// TODO: actual logic
void loop() {
 Door::openDoor();
 Door::closeDoor();
 Door::getDoorStatus();
 GPS::getCurrentUtcTime();
 Logging::logMissionData(GPS::getRawGPSData());

 Sensors::PressureData pressure = Sensors::getPressureData();
 Logging::logMissionData(
   "$HAB_PRESSURE," +
   String(pressure.raw) + "," +
   String(pressure.bar)
 );

// Log Temperature
  Sensors::TemperatureData onboardTemperature = Sensors::onboardThermistor.getTemperature();
  Logging::logMissionData(
    "$HAB_TEMPERATURE_ONBOARD," +
    String(onboardTemperature.raw) + "," +
    String(onboardTemperature.tempC) + "," +
    String(onboardTemperature.tempF)
  );

  Sensors::TemperatureData outdoorTopTemperature = Sensors::outdoorTopThermistor.getTemperature();
  Logging::logMissionData(
    "$HAB_TEMPERATURE_OUTDOOR_TOP," +
    String(outdoorTopTemperature.raw) + "," +
    String(outdoorTopTemperature.tempC) + "," +
    String(outdoorTopTemperature.tempF)
  );

  Sensors::TemperatureData outdoorBottomTemperature = Sensors::outdoorBottomThermistor.getTemperature();
  Logging::logMissionData(
    "$HAB_TEMPERATURE_OUTDOOR_BOTTOM," +
    String(outdoorBottomTemperature.raw) + "," +
    String(outdoorBottomTemperature.tempC) + "," +
    String(outdoorBottomTemperature.tempF)
  );

 if (GPS::process()) {
   Logging::logSystemData(GPS::getRawGPSData());
   Logging::logSystemData(String(GPS::getCurrentAltitude()));
 }
   delay(1000);
}
