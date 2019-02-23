/**
 * The GPS module provides data in the GPGGA and GPRMC sentence format,
 * which contain a number of comma-separated values.
 *
 * See the following links for information about those format:
 * https://docs.novatel.com/OEM7/Content/Logs/GPGGA.htm
 * https://docs.novatel.com/OEM7/Content/Logs/GPRMC.htm
 */

#ifndef GPS_H
#define GPS_H

#include <WString.h>

bool isGPRMC(String gpsData);
bool isGPGGA(String gpsData);
bool isGPSData(String gpsData);
float getAltitude(String gpsData);

/**
 * Previously the GPS data was parsed on the fly, stored in this struct, and then
 * written out to disk. The data is no longer parsed into this struct, but it serves
 * as a good reference for the GPGGA fields.
 */
struct GPGGA_Data {
  // The full NMEA sentence, unparsed
  String full;

  // The sentence header, identifying the type of NMEA sentence. We're listening for a $GPGGA header.
  // Type: string
  // Value index 0
  String type;

  // UTC time, in format: hhmmss.ss
  // Type: float
  // Value index 1
  String utc_time;

  // Current latitude, in format: DDmm.mm (e.g. 4124.8963 = 41° 24.8963')
  // Type: float
  // Value index 2
  String lat;

  // Latitude direction (N or S)
  // Type: char
  // Value index 3
  String latDir;

  // Current longitude, in format: DDDmm.mm (e.g. 08151.6838 = 81° 51.6838')
  // Type: float
  // Value index 4
  String lng;

  // Longitude direction (E or W)
  // Type: char
  // Value index 5
  String lngDir;

  // GPS quality indicator
  // See: https://docs.novatel.com/OEM7/Content/Logs/GPGGA.htm#GPSQualityIndicators
  // Type: int
  // Value index 6
  String quality;

  // Number of satellites in use
  // Type: int
  // Value index 7
  String numOfSatellites;

  // Horizontal dilution of precision
  // Type: float
  // Value index 8
  String HDOP;

  // Altitude above/below mean sea level
  // Type: float
  // Value index 9
  String altitude;

  // Units of antenna altitude, currently unused (M = meters)
  // Type: char
  // Value index 10
  // String altitudeUnits;

  // Undulation - relationship between the geoid and WGS84 ellipsoid
  // Type: float
  // Value index 11
  String WGS84_ellipsoid;

  // Units of undulation (M = meters), currently unused
  // Type: char
  // Value index 12
  // String undulationUnits;

  // Age of correction data in seconds, currently unused
  // Type: int
  // Value index 13

  // Differential base station ID (may be empty)
  // Type: string
  // Value index 14
  String DGPS_reference;

  // Checksum (format: *hh, e.g. *48)
  // Type: string
  // Value index 15
  String checkSum;
};

#endif
