#include <stdio.h>
#include "GPS.h"

bool isGPGGA(String gpsData) {
  return gpsData.startsWith("$GPGGA");
}

bool isGPRMC(String gpsData) {
  return gpsData.startsWith("$GPRMC");
}

// We're only interested in the GPGGA and GPRMC sentences from the GPS module
bool isGPSData(String gpsData) {
  return isGPGGA(gpsData) || isGPRMC(gpsData);
}

// There may be a better way to implement this, and it needs to be tested
float getAltitude(String gpsData) {
  String values[16];
  char buf[gpsData.length()];
  uint8_t i = 0;

  // Convert the String object to a char array to be parsed by strtok
  gpsData.toCharArray(buf, gpsData.length());

  // Split the data by commas
  char* ptr = strtok(buf, ",");
  while (ptr != NULL) {
    values[i] = ptr;
    i++;
    ptr = strtok(NULL, ",");
  }

  float altitude = values[9].toFloat();
  return altitude;
}
