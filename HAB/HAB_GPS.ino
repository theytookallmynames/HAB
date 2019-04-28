#include <MicroNMEA.h>
#include "HAB_GPS.h"
#include "HAB_Logging.h"

namespace HAB {
namespace GPS {

char gpsBuffer[200];
MicroNMEA nmea(gpsBuffer, sizeof(gpsBuffer));
bool didInit = false;

bool init() {
  pinMode(GPS_STATUS_LED_PIN, OUTPUT);
  GPSSerial.begin(4800);
  while (!GPSSerial);

  Logging::logSystemData("GPS serial ready");
  Logging::logSystemData("Waiting for GPS acquisition");

  GPSSerial.println(PMTK_SET_NMEA_UPDATE_1HZ);
  GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  delay(1000);

  uint32_t gpsTimeout = millis();

  while (true) {
    if (GPSSerial.available()) {
      // We need to wait for a GPRMC message to come in, since that contains the full UTC time and date.
      if (process() && strcmp(nmea.getMessageID(), "RMC") == 0) {
        GPSTimeData time = getCurrentUtcTime();
        Logging::logSystemData(
          "GPS signal acquired at " +
          String(time.year) + "-" +
          String(time.month) + "-" +
          String(time.day) + " " +
          String(time.hour) + ":" +
          String(time.minute) + ":" +
          String(time.seconds) + " UTC"
        );
        break;
      }

      // Flash the GPS status LED to indicate that we're waiting for GPS acquisition
      digitalWrite(GPS_STATUS_LED_PIN, HIGH);
      delay(400);
      digitalWrite(GPS_STATUS_LED_PIN, LOW);
      delay(400);
    }

    if (millis() - gpsTimeout > GPS_MAX_WAIT) {
      Logging::logSystemData(
        "GPS acquisition failed. Timed out after " +
        String(GPS_MAX_WAIT) +
        " ms. Aborting."
      );
      return false;
    }
  }

  didInit = true;
  digitalWrite(GPS_STATUS_LED_PIN, HIGH);
  return true;
}

/**
 * Read and process data from the GPS serial module.
 * Returns true if a complete NMEA sentence has been processed.
 */
bool process() {
  while (GPSSerial.available()) {
    char c = GPSSerial.read();
    bool isValid = nmea.process(c) && nmea.isValid();

    if (isValid) {
      latestTime.year = nmea.getYear();
      latestTime.month = nmea.getMonth();
      latestTime.day = nmea.getDay();
      latestTime.hour = nmea.getHour();
      latestTime.minute = nmea.getMinute();
      latestTime.seconds = nmea.getSecond();
    }

    // nmea.process(c) returns true if a complete NMEA sentence has been read
    return isValid;
  }
}

/**
 * Returns time and date data from the GPS
 */
GPSTimeData getCurrentUtcTime() {
  return latestTime;
}

/**
 * Returns the current altitude in meters.
 * If a GPS fix is not available, returns -1.
 */
long getCurrentAltitude() {
  long alt;

  if (nmea.getAltitude(alt)) {
    return alt;
  }

  return -1;
}

/**
 * Returns the latest full NMEA sentence received from the GPS module.
 * If no valid sentence has been received, returns an empty string.
 */
const char* getRawGPSData() {
  if (nmea.isValid()) {
    return nmea.getSentence();
  }
  return "";
}

bool gpsReady() {
  return didInit;
}

} // namespace GPS
} // namespace HAB
