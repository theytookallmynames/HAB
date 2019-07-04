#include <NMEAGPS.h>
#include "HAB_GPS.h"
#include "HAB_Logging.h"
#include "HAB_LED.h"

namespace HAB {
namespace GPS {

NMEAGPS gps;

bool didInit = false;

bool init() {
  GPSSerial.begin(4800);
  while (!GPSSerial);

  Logging::logSystemData("GPS serial ready");
  Logging::logSystemData("Waiting for GPS acquisition");
  LED::statusLED(GPS_STATUS_LED, LED::pending);

  GPSSerial.println(PMTK_SET_NMEA_UPDATE_1HZ);
  GPSSerial.println(PMTK_SET_NMEA_OUTPUT_RMCGGA);

  delay(1000);

  uint32_t gpsTimeout = millis();
  while (true) {
    GPSData data = process();
    if (data.isValid) {
      // We need to wait for a GPRMC message to come in, since that contains the full UTC time and date.
        Logging::logSystemData(
          "GPS signal acquired at " +
          data.time.description()
        );
        break;
    }

    if (millis() - gpsTimeout > GPS_MAX_WAIT) {
      Logging::logSystemData(
        "GPS acquisition failed. Timed out after " +
        String(GPS_MAX_WAIT) +
        " ms. Aborting."
      );
      LED::statusLED(GPS_STATUS_LED, LED::failure);
      return false;
    }
  }

  didInit = true;
  LED::statusLED(GPS_STATUS_LED, LED::success);
  return true;
}

/**
 * Read and process data from the GPS serial module.
 * Returns true if a complete NMEA sentence has been processed.
 */
//String sentence = "";
GPSData process() {

  while (gps.available(GPSSerial)) {
//    char c = GPSSerial.read()
//    sentence += c;
//
//    if (gps.decode(c) == NMEAGPS::DECODE_COMPLETED) {
//      
//    }
    
    gps_fix fix = gps.read();
    bool isValid = fix.valid.time && fix.valid.date && fix.valid.location && fix.valid.altitude;
    if (isValid) {
        _data.time.year = fix.dateTime.year;
        _data.time.month = fix.dateTime.month;
        _data.time.day = fix.dateTime.date;
        _data.time.hour = fix.dateTime.hours;
        _data.time.minute = fix.dateTime.minutes;
        _data.time.seconds = fix.dateTime.seconds;
        _data.latitude = fix.latitudeL();
        _data.longitude = fix.longitudeL();
        _data.speed = fix.speed_kph();
        _data.nmeaSentence = gps.string_for(gps.nmeaMessage);
        _data.altitude = fix.altitude() ? : -1;
        _data.numberOfSatellites = fix.satellites;
        _data.isValid = true;
        return _data;
    }
  }

  _data.isValid = false;
  return _data;
}

/**
 * Returns time and date data from the GPS
 */
GPSTimeData getCurrentUtcTime() {
  return _data.time;
}


bool isGpsReady() {
  return didInit;
}

} // namespace GPS
} // namespace HAB
