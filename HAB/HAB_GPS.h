#ifndef HAB_GPS_H
#define HAB_GPS_H

namespace HAB {
namespace GPS {

// The GPS module communicates over hardware serial.
#define GPSSerial Serial1

// Max time to wait for initial GPS acquisition in milliseconds
#define GPS_MAX_WAIT 20000

// Instruction to tell GPS to only output GPRMC and GPGGA data, from Adafruit_GPS.h
#define PMTK_SET_NMEA_OUTPUT_RMCGGA "$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"

// Instruction to tell GPS how often to output NMEA sentences, from Adafruit_GPS.h
#define PMTK_SET_NMEA_UPDATE_5HZ  "$PMTK220,200*2C" // 5 times per second
#define PMTK_SET_NMEA_UPDATE_1HZ  "$PMTK220,1000*1F" // 1 time per second

struct GPSTimeData {
  uint16_t year;
  uint8_t month;
  uint8_t day;
  uint8_t hour;
  uint8_t minute;
  uint8_t seconds;
};

GPSTimeData latestTime;

bool init();
GPSTimeData getCurrentUtcTime();
const char* getRawGPSData();
bool gpsReady();
bool process();
long getCurrentAltitude();

} // namespace GPS
} // namespace HAB

#endif // HAB_GPS_H
