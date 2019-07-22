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
  /**
   * Door initialization is accomplished by visual inspection of the doors opening and closing.
   */
  //Door::init();

  if (!gpsInitSuccess || !loggingInitSuccess || !sensorInitSuccess) {
    Logging::logSystemData("A subsystem failed to initialize properly. Aborting.");
    LED::statusLED(SYSTEM_STATUS_LED, LED::failure);
    return;
  }


  Logging::logSystemData("All subsystems initialized, starting main loop.");
  LED::statusLED(SYSTEM_STATUS_LED, LED::success);
}

void loop() {
  GPS::GPSData gpsData = GPS::process();
  
  Door::DoorStatus door1Status = Door::getDoorStatus1();
  Door::DoorStatus door2Status = Door::getDoorStatus2();

  Sensors::TemperatureData onboardTemperature = Sensors::onboardThermistor.getTemperature();
  Sensors::TemperatureData outdoorTopTemperature = Sensors::outdoorTopThermistor.getTemperature();
  Sensors::TemperatureData outdoorBottomTemperature = Sensors::outdoorBottomThermistor.getTemperature();
  Sensors::PressureData pressure = Sensors::getPressureData();

  Logging::MissionData _data;
    
  _data.setOnboardTemperature(onboardTemperature);
  _data.setOutdoorTopTemperature(outdoorTopTemperature);
  _data.setOutdoorBottomTemperature(outdoorBottomTemperature);
  _data.setPressureData(pressure);
  _data.setDoor1Status(door1Status);
  _data.setDoor2Status(door2Status);
  
  Logging::logMissionData(_data.toString());
    
    if (gpsData.isValid) {
      adjustDoorIfNeeded(gpsData.altitude);
      _data.setGpsData(gpsData);
    }
   
    Logging::logSystemData(GPS::_data.nmeaSentence);
    
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
