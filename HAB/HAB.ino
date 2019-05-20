#include "HAB.h"
#include "HAB_GPS.h"
#include "HAB_Logging.h"
#include "HAB_Sensors.h"
#include "HAB_Door.h"
#include "HAB_Thermistor.h"
using namespace HAB;

Thermistor indoorThermistor = Thermistor(TEMPERATURE_ONBOARD_PIN);
Thermistor outdoorTopThermistor = Thermistor(TEMPERATURE_OUTSIDE_TOP_PIN);
Thermistor outdoorBottomThermistor = Thermistor(TEMPERATURE_OUTSIDE_BOTTOM_PIN);

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
  TemperatureData indoorTemperature = indoorThermistor.getTemperature();
  Logging::logMissionData(
    "$HAB_TEMPERATURE_INDOOR," +
    String(indoorTemperature.raw) + "," +
    String(indoorTemperature.tempC) + "," +
    String(indoorTemperature.tempF)
  );

  TemperatureData outdoorTopTemperature = outdoorTopThermistor.getTemperature();
  Logging::logMissionData(
    "$HAB_TEMPERATURE_OUTDOOR_TOP," +
    String(outdoorTopTemperature.raw) + "," +
    String(outdoorTopTemperature.tempC) + "," +
    String(outdoorTopTemperature.tempF)
  );

  TemperatureData outdoorBottomTemperature = outdoorBottomThermistor.getTemperature();
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
