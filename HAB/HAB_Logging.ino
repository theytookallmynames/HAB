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

  if (!SD.begin()) {
    logSystemData("SD card initialization failed. Could not connect to SD card.");
    return false;
  }

  logSystemData("SD card ready");

  if (!GPS::gpsReady()) {
    logSystemData("Logging::init() was run before GPS::init(), and therefore data files cannot be created. Aborting.");
    return false;
  }

  String timestamp = createTimestamp();
  missionLogFileName = timestamp + "_mission.csv";
  systemLogFileName = timestamp + "_system.txt";

  didInit = true;
  digitalWrite(SD_STATUS_LED_PIN, HIGH);

  logSystemData("Log files created.");
  logSystemData("Logging mission data to " + missionLogFileName);
  logSystemData("Logging system data to " + systemLogFileName);
  return true;
}

/**
 * Log system messages such as debug messages, errors, etc.
 * Prints the message and a newline.
 */
void logSystemData(String data) {
  String timestamp = "";

  // if (GPS::gpsReady()) {
  //   timestamp = createTimestamp();
  // }

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
    File file = SD.open(systemLogFileName);
    if (file) {
      if (timestamp.length() != 0) {
        file.print(timestamp); file.print(" ");
      }

      file.println(data);
      file.close();
    }
  }
}

/**
 * Log mission data such as GPS and sensors readings.
 */
void logMissionData(String data) {
  // if (Serial) {
  //   if (GPS::gpsReady()) {
  //     String timestamp = createTimestamp();
  //     Serial.println(timestamp + " " + data);
  //   } else {
  //     Serial.println(data);
  //   }
  // }

  File file = SD.open(missionLogFileName, FILE_WRITE);
  if (file) {
    file.println(data);
    file.close();
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

} // namespace Logging
} // namespace HAB
