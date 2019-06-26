#include "HAB.h"
#include "HAB_GPS.h"
#include "HAB_Logging.h"
#include "HAB_Sensors.h"
#include "HAB_Thermistor.h"
#include "HAB_Door.h"
#include "HAB_LED.h"
using namespace HAB;


void setup() {
  Serial.begin(9600);

// Block until the serial monitor is connected in development
#ifdef HAB_DEVELOPMENT
  while(!Serial);
#endif 

  Logging::logSystemData("System startup");

  LED::init();
  bool gpsInitSuccess = GPS::init();
  bool loggingInitSuccess = Logging::init();
  bool sensorInitSuccess = Sensors::init();
  Door::init();

  if (!gpsInitSuccess || !loggingInitSuccess || !sensorInitSuccess) {
    Logging::logSystemData("A subsystem failed to initialize properly. Aborting.");
    LED::statusLED(SYSTEM_STATUS_LED, LED::failure);
    return;
  }

  Logging::logSystemData("All subsystems initialized, starting main loop.");
  LED::statusLED(SYSTEM_STATUS_LED, LED::success);
}

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
//   Sensors::PressureData pressure;
//   pressure.raw = 2;
//   pressure.bar = 4;

  Logging::MissionData data;
  data.setRawGpsData(rawGpsData);
  data.setGpsAltitude(gpsAltitude);
  data.setOnboardTemperature(onboardTemperature);
  data.setOutdoorTopTemperature(outdoorTopTemperature);
  data.setOutdoorBottomTemperature(outdoorBottomTemperature);
  data.setPressureData(pressure);
  data.setDoor1Status(door1Status);
  data.setDoor2Status(door2Status);
  
  Logging::logMissionData(data.toString());
  adjustDoorIfNeeded(gpsAltitude);
  
 if (GPS::process()) {
   Logging::logSystemData(GPS::getRawGPSData());
   Logging::logSystemData(String(GPS::getCurrentAltitude()));
 }
   delay(1000);
}

//Doors opening and closing at 60000 ft

void adjustDoorIfNeeded(long altitude) {
  if (altitude >= MOTOR_OPEN_TRIGGER_ALTITUDE) {
      Door::openDoor();
      
      //TODO: System log the door opening time and altitude
  } else if (altitude <= MOTOR_CLOSE_TRIGGER_ALTITUDE) {
      Door::closeDoor();
      //TODO: System log the door closing time stamp and altitude
  }
}
