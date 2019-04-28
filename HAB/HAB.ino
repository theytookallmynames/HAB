#include "HAB.h"
#include "HAB_GPS.h"
#include "HAB_Logging.h"
#include "HAB_Sensors.h"
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

  Sensors::TemperatureData onboardTemp = Sensors::getOnboardTemperature();
  Logging::logMissionData(
    "$HAB_TEMPERATURE," +
    String(onboardTemp.raw) + "," +
    String(onboardTemp.tempC) + "," +
    String(onboardTemp.tempF)
  );

  if (GPS::process()) {
    Logging::logSystemData(GPS::getRawGPSData());
    Logging::logSystemData(String(GPS::getCurrentAltitude()));
  }

  delay(1000);
}
