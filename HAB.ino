// Based on Ladyada & Bill Greimanfor's logger
// for the Adafruit Ultimate GPS Shield
// Written by Sadik F. Erisen and Vladyslav Kochetkov
// Modified and finished on August 8th 2018

#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "StemHab.h"




// Data wire is plugged into pin 14 on the Arduino
#define ONE_WIRE_BUS 14

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);



const int MOTOR_TRIGGER_ALTITUDE = 46;
int delta, y_1, y_2 =0;

void SDwrite();
void openDoor();
void closeDoor();

const int chipSelect = 4;
bool motor_flag = false;

SoftwareSerial mySerial(10, 9);
int motor_one_p1 = A4;
int motor_one_p2 = 12;
int motor_two_p1 = A5;
int motor_two_p2 = 11;

String singleLine = "";

File logfile;
String fileName = "GPSLOG";
bool flag = false;
bool done = false;

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  // Start up the library - Dallas Temperature
  sensors.begin();
  while (!Serial) {
    ;
  }


  pinMode(chipSelect, OUTPUT);
  pinMode(motor_one_p1, OUTPUT);
  pinMode(motor_one_p2, OUTPUT);
  pinMode(motor_two_p1, OUTPUT);
  pinMode(motor_two_p2, OUTPUT);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }
  Serial.println("card initialized.");

  for (uint8_t i = 0; i < 1000; i++) {
    if (!SD.exists(fileName + i + ".csv")) {
      fileName = fileName + i + ".csv";
      break;
    }
  }

  SDwrite("Type,Time,Latitude,Longitude,Quality,Number Of Satellites,Horizontal Dilution of Precision (HDOP),Altitude,Height of geoid above WGS84 ellipsoid,DGPS reference station id,Checksum,Temp 1 (0), Temp 1 (Index 0 RAW), Temp 2 (1), Temp 2 (Index 1 RAW),Temp 3 (2), Temp 2 (Index 2 RAW),Door Status");
  mySerial.begin(4800);
  Serial.println("Ready!");
}

struct singleLineParsed {
  String full;
  String type;
  String time;
  String lat;
  String lng;
  String quality;
  String numOfSatellites;
  String HDOP;
  String altitude;
  String WGS84_ellipsoid;
  String DGPS_reference;
  String checkSum;
};

struct altitudeAverage {
    int altitudes[60];
    int currentLocation = 0;
    double average;

    altitudeAverage(){
        for(int i = 0; i < 60; i++){
            altitudes[i] = -1;
        }
    }

    void addAltitude(const int newVal){
        altitudes[currentLocation] = newVal;
        currentLocation++;
        if(currentLocation == 60){
            currentLocation = 0;
        }
    }

    int getAverage(){
        int total;
        for(int i = 0; i < 60; i++){
            if(altitudes[i] !== -1)
                total += altitudes[i];
        }
        return total/60;
    }
}

void SDwrite(String x) {
  Serial.println(x);
  logfile = SD.open(fileName, FILE_WRITE);
  logfile.println(x);
  logfile.close();
}

void openDoor() {
  flag = true;
  digitalWrite(motor_one_p1, HIGH);
  digitalWrite(motor_one_p2, LOW);
  digitalWrite(motor_two_p1, HIGH);
  digitalWrite(motor_two_p2, LOW);
}

void closeDoor() {
  digitalWrite(motor_one_p1, LOW);
  digitalWrite(motor_one_p2, HIGH);
  digitalWrite(motor_two_p1, LOW);
  digitalWrite(motor_two_p2, HIGH);
}

void loop() {

  if (mySerial.available()) {
    char x = (char) mySerial.read();
    if (x == '\n') {
      singleLineParsed newLine;
      if (singleLine.substring(0, 10).indexOf("$GPGGA") != -1) {
        for (int i = 0; i < 20; i++) {
          if (i == 0) {
            newLine.full = singleLine;
          }
          int split = singleLine.indexOf(",");
          String y = singleLine.substring(0, split);
          singleLine = singleLine.substring(split + 1, singleLine.length());
          switch (i) {
            case 0:
              newLine.type =  y;
              break;
            case 1:
              newLine.time = y;
              break;
            case 2:
              newLine.lat = y;
              break;
            case 3:
              break;
            case 4:
              newLine.lng = y;
              break;
            case 5:
              break;
            case 6:
              newLine.quality = y;
              break;
            case 7:
              newLine.numOfSatellites = y;
              break;
            case 8:
              newLine.HDOP = y;
              break;
            case 9:
              newLine.altitude = y;
              break;
            case 10:
              break;
            case 11:
              newLine.WGS84_ellipsoid = y;
              break;
            case 12:
              break;
            case 13:
              break;
            case 14:
              newLine.DGPS_reference = singleLine.substring(0, 4);
              newLine.checkSum = singleLine.substring(4, singleLine.length() - 1);

              y_2 = newLine.altitude.toInt();
              delta = y_2 - y_1;
              y_1 = y_2;

              if(y_1 > MOTOR_TRIGGER_ALTITUDE){
                  SDwrite(
                      newLine.type + "," + newLine.time + "," + newLine.lat + "," + newLine.lng + "," + newLine.quality + "," + newLine.numOfSatellites + "," + newLine.HDOP + "," + newLine.altitude + "," + newLine.WGS84_ellipsoid + "," + newLine.DGPS_reference + "," + newLine.checkSum + "," + sensors.getTempCByIndex(0) + "," + sensors.getTempCByIndex(0) + "," + sensors.getTempCByIndex(1) + "," + sensors.getTempCByIndex(1) + "," +
                      sensors.getTempCByIndex(2) + "," + sensors.getTempCByIndex(2) + "," +
                      "1" + "," + delta
                  );
                  openDoor();
              }else{
                    SDwrite(
                        newLine.type + "," + newLine.time + "," + newLine.lat + "," + newLine.lng + "," + newLine.quality + "," + newLine.numOfSatellites + "," + newLine.HDOP + "," + newLine.altitude + "," + newLine.WGS84_ellipsoid + "," + newLine.DGPS_reference + "," + newLine.checkSum + "," + sensors.getTempCByIndex(0) + "," + sensors.getTempCByIndex(0) + "," + sensors.getTempCByIndex(1) + "," + sensors.getTempCByIndex(1) + "," +
                        sensors.getTempCByIndex(2) + "," + sensors.getTempCByIndex(2) + "," + "0" + "," + delta
                    );
                    if(flag){
                      closeDoor();
                    }
              }
              break;
          }

          if (singleLine.length() == 0)
            break;
        }
      }
      singleLine = "";
    } else {
      singleLine += x;
    }
  }
}
