#include <SD.h>
#include "HAB.h"
#include "HAB_GPS.h"
#include "HAB_Logging.h"

namespace HAB {
namespace Logging {

String createTimestamp();

bool didInit = false;
String missionLogFileName;
String systemLogFileName;

/**
 * Initialize the SD card and create the necessary files.
 * This must be run after GPS::init() since the filenames depend on the UTC timestamp.
 */
bool init() {
  pinMode(SD_CARD_PIN, OUTPUT);
  pinMode(SD_STATUS_LED_PIN, OUTPUT);

  if (!SD.begin(SD_CARD_PIN)) {
    logSystemData("SD card initialization failed. Could not connect to SD card.");
    return false;
  }

  logSystemData("SD card ready");

  if (!GPS::gpsReady()) {
    logSystemData("Logging::init() was run before GPS::init(), and therefore data files cannot be created. Aborting.");
    return false;
  }

  GPS::GPSTimeData time = GPS::getCurrentUtcTime();
  
  //limited to 8 charachters in file name and three characters in extension
  missionLogFileName = String(time.day) + String (time.hour) + String (time.minute) + String (time.seconds) + ".csv";
  systemLogFileName = String (time.day) + String (time.hour) + String (time.minute) + String (time.seconds) + ".log";

  File systemFile = SD.open(systemLogFileName, FILE_WRITE);
  File missionFile = SD.open(missionLogFileName, FILE_WRITE);
  if (!systemFile || !missionFile){
    Serial.println("File not Created. Aborting...");
    return false;
  }
  
  // Files are created, set the title of the CSV and write on systemFile
  missionFile.println(MissionData::getTitles());
  systemFile.println("System File initializing...");

  // Read files to check if data was recorded.
  char firstMissionChar = missionFile.read();
  char firstSystemChar = systemFile.read();
  // Serial.println("Char is");
  // Serial.println(firstSystemChar);
  // Serial.println(firstMissionChar);

  if (firstMissionChar == -1) {
      Serial.println("Can't read from missionFile. Aborting...");
      return false;
  } else if (firstSystemChar == -1) {
      Serial.println("Can't read from system file. Aborting...");
      return false;
  }

  missionFile.close();
  systemFile.close();
  
  logSystemData("Log files created.");
  logSystemData("Logging mission data to " + missionLogFileName);
  logSystemData("Logging system data to " + systemLogFileName);
  logSystemData("Logging module is ready for deployment...");

  didInit = true;
  digitalWrite(SD_STATUS_LED_PIN, HIGH);

  return true;
}

/**
 * Log system messages such as debug messages, errors, etc.
 * Prints the message and a newline.
 */
void logSystemData(String data) {
  String timestamp = "";

  if (GPS::gpsReady()) {
     timestamp = createTimestamp();
  }

  if (Serial) {
    // Serial.print("timestamp length: ");
    // Serial.println(timestamp.length(), DEC);

    if (timestamp.length() != 0) {
      Serial.print(timestamp); Serial.print(" ");
    }

    Serial.println(data);
  }

  // logSystemData may be called before Logging::init() is finished,
  // so we need to check if the SD card has been initialized yet before
  // writing to it.
  if (didInit) {
    File file = SD.open(systemLogFileName, FILE_WRITE);
    if (file) {
      Serial.println("File exists");
      if (timestamp.length() != 0) {
        file.print(timestamp); file.print(" ");
      }

      file.println(data);
      file.close();
    }
    else {
      Serial.println("Could not open file " + systemLogFileName);
    }
  }
}
/**
 * Log mission data such as GPS and sensors readings.
 */

 //TODO: rename logMissionData so they are not same
 
void logMissionData(String data) {
  logSystemData("Logging missionData.");
  File file = SD.open(missionLogFileName, FILE_WRITE);
  if (file) {
    file.println(data);
    file.close();
  } else {
    Serial.println("Could not open file " + missionLogFileName);
  }
}

/**
 * Returns a timestamp from GPS data in the format YYYY-MM-DD_hh-mm-ss.
 */
String createTimestamp() {
  GPS::GPSTimeData time = GPS::getCurrentUtcTime();

  return String(
    String(time.year) + "-" +
    String(time.month) + "-" +
    String(time.day) + "_" +
    String(time.hour) + "-" +
    String(time.minute) + "-" +
    String(time.seconds)
  );
}

  void MissionData::setRawGpsData(const char* rawGpsData) {
      data.rawGpsData = rawGpsData;
  }
  void MissionData::setGpsAltitude (long gpsAltitude) { 
      data.gpsAltitude = gpsAltitude;
  }
  void MissionData::setOnboardTemperature(Sensors::TemperatureData onboardTemperature) {
      data.onboardTemperature = onboardTemperature;
  }
  void MissionData::setOutdoorTopTemperature(Sensors::TemperatureData outdoorTopTemperature) {
      data.outdoorTopTemperature = outdoorTopTemperature;
  }
  void MissionData::setOutdoorBottomTemperature(Sensors::TemperatureData outdoorBottomTemperature) {
      data.outdoorBottomTemperature = outdoorBottomTemperature;
  }
  void MissionData::setPressureData(Sensors::PressureData pressure){
     data.pressure = pressure;
  }
  void MissionData::setDoor1Status(Door::DoorStatus door1Status){
     data.door1Status = door1Status;
  }
  void MissionData::setDoor2Status(Door::DoorStatus door2Status){
     data.door2Status = door2Status;
  }


  String MissionData::getTitles() {
      String titles = "date,"
                "rawGpsData,"
                "gpsAltitude,"
                "pressureRaw,"
                "pressureBar,"
                "onboardTempRaw,"
                "onboardTempC,"
                "onboardTempF,"
                "outdoorTopTempRaw,"
                "outdoorTopTempC,"
                "outdoorTopTempF,"
                "outdoorBottomTempRaw,"
                "outdoorBottomTempC,"
                "outdoorBottomTempF,"
                "door1Status,"
                "door2Status";
    return titles;
  }

  String MissionData::toString() {
      String gpsData = String(data.rawGpsData);
      
      //csv creates a column for each GPS number because they are comma separated, this replaces all commas with -
      gpsData.replace(',','-');
      
      // Creating comma separated sentence for logging into csv file.
      String missionData = createTimestamp() + "," + 
                       gpsData + "," +
                       String(data.gpsAltitude) + "," +
                       String(data.pressure.raw) + "," + 
                       String(data.pressure.bar) + "," +
                       String(data.onboardTemperature.raw) + "," + 
                       String(data.onboardTemperature.tempC) + "," + 
                       String(data.onboardTemperature.tempF) + "," +
                       String(data.outdoorTopTemperature.raw) + "," + 
                       String(data.outdoorTopTemperature.tempC) + "," + 
                       String(data.outdoorTopTemperature.tempF) + "," +
                       String(data.outdoorBottomTemperature.raw) + "," + 
                       String(data.outdoorBottomTemperature.tempC) + "," + 
                       String(data.outdoorBottomTemperature.tempF) + "," +
                       String(data.door1Status) + "," +
                       String(data.door2Status);
    return missionData;
  }
} // namespace Logging
} // namespace HAB
