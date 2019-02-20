/**
 * This file is the entrypoint for the Arduino board.
 * Based on Ladyada & Bill Greimanfor's logger for the Adafruit Ultimate GPS Shield.
 * Authors:
 *  - Sadik F. Erisen
 *  - Vladyslav Kochetkov
 *  - Vito LaVilla <vito@vitosamson.com>
 */

/**
 * Writing to the SD card.
 * https://www.arduino.cc/en/reference/SD
 */
#include <SD.h>

/**
 * Communication over the serial interface.
 * https://www.arduino.cc/en/Reference/softwareSerial
 */
#include <SoftwareSerial.h>

/**
 * Reads data from the pressure sensors.
 * https://www.pjrc.com/teensy/td_libs_OneWire.html
 * https://playground.arduino.cc/Learning/OneWire
 */
#include <OneWire.h>

/**
 * Reads data from the temperature sensors.
 * https://github.com/milesburton/Arduino-Temperature-Control-Library
 * https://create.arduino.cc/projecthub/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806#about-project
 */
#include <DallasTemperature.h>

// The data wire is connected to pin 14 on the Arduino
#define ONE_WIRE_BUS 14

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// The altitude (in km?) at which the door should open and close.
const int MOTOR_TRIGGER_ALTITUDE = 46;
int delta, y_1, y_2 =0;

void SDwrite();
void openDoor();
void closeDoor();

// SD card pin?
const int chipSelect = 4;

// current open state of the door. true if open, false if closed.
bool doorOpen = false;

// receive pin = 10, transmit pin = 9
SoftwareSerial mySerial(10, 9);

int motor_one_pin_1 = PIN_A4;
int motor_one_pin_2 = 12;
int motor_two_pin_1 = PIN_A5;
int motor_two_pin_2 = 11;

// data from the serial input is read into here until it's ready to be parsed
String serialBuffer = "";

File logfile;
String logFileName = "GPSLOG";

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");

  // Start up the library - Dallas Temperature
  sensors.begin();

  // block until the serial port is available
  while (!Serial) {
    ;
  }

  // set all pins to OUTPUT mode
  // https://www.arduino.cc/reference/en/language/functions/digital-io/pinmode/
  pinMode(chipSelect, OUTPUT);
  pinMode(motor_one_pin_1, OUTPUT);
  pinMode(motor_one_pin_2, OUTPUT);
  pinMode(motor_two_pin_1, OUTPUT);
  pinMode(motor_two_pin_2, OUTPUT);

  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    return;
  }

  Serial.println("card initialized.");

  for (uint8_t i = 0; i < 1000; i++) {
    if (!SD.exists(logFileName + i + ".csv")) {
      logFileName = logFileName + i + ".csv";
      break;
    }
  }

  SDwrite("Type,Time,Latitude,Longitude,Quality,Number Of Satellites,Horizontal Dilution of Precision (HDOP),Altitude,Height of geoid above WGS84 ellipsoid,DGPS reference station id,Checksum,Temp 1 (0), Temp 1 (Index 0 RAW), Temp 2 (1), Temp 2 (Index 1 RAW),Temp 3 (2), Temp 2 (Index 2 RAW),Door Status");

  mySerial.begin(4800);
  Serial.println("Ready!");
}

// the serial data is parsed into the struct
// TODO: document what each field is, its format, etc.
struct SensorData {
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

// not used
/* struct altitudeAverage {
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
}; */

void SDwrite(String x) {
  Serial.println(x);
  logfile = SD.open(logFileName, FILE_WRITE);
  logfile.println(x);
  logfile.close();
}

void openDoor() {
  doorOpen = true;
  digitalWrite(motor_one_pin_1, HIGH);
  digitalWrite(motor_one_pin_2, LOW);
  digitalWrite(motor_two_pin_1, HIGH);
  digitalWrite(motor_two_pin_2, LOW);
}

void closeDoor() {
  digitalWrite(motor_one_pin_1, LOW);
  digitalWrite(motor_one_pin_2, HIGH);
  digitalWrite(motor_two_pin_1, LOW);
  digitalWrite(motor_two_pin_2, HIGH);
}

void loop() {
  if (mySerial.available()) {
    char serialChar = (char) mySerial.read();

    // if we've reached the end of a line of serial input, parse it.
    if (serialChar == '\n') {
      SensorData data;

      if (serialBuffer.substring(0, 10).indexOf("$GPGGA") != -1) {
        // the serial data contains 20 values separated by commas
        for (int valueIndex = 0; valueIndex < 20; valueIndex++) {
          if (valueIndex == 0) {
            data.full = serialBuffer;
          }

          // find the first value in the buffered string
          int split = serialBuffer.indexOf(",");
          String value = serialBuffer.substring(0, split);

          // splice out the current value in preparation for the next loop iteration
          serialBuffer = serialBuffer.substring(split + 1, serialBuffer.length());

          switch (valueIndex) {
            case 0:
              data.type =  value;
              break;
            case 1:
              data.time = value;
              break;
            case 2:
              data.lat = value;
              break;
            case 3:
              break;
            case 4:
              data.lng = value;
              break;
            case 5:
              break;
            case 6:
              data.quality = value;
              break;
            case 7:
              data.numOfSatellites = value;
              break;
            case 8:
              data.HDOP = value;
              break;
            case 9:
              data.altitude = value;
              break;
            case 10:
              break;
            case 11:
              data.WGS84_ellipsoid = value;
              break;
            case 12:
              break;
            case 13:
              break;
            case 14:
              data.DGPS_reference = serialBuffer.substring(0, 4);
              data.checkSum = serialBuffer.substring(4, serialBuffer.length() - 1);

              y_2 = data.altitude.toInt();
              delta = y_2 - y_1;
              y_1 = y_2;

              if (y_1 > MOTOR_TRIGGER_ALTITUDE) {
                SDwrite(
                  data.type + "," +
                  data.time + "," +
                  data.lat + "," +
                  data.lng + "," +
                  data.quality + "," +
                  data.numOfSatellites + "," +
                  data.HDOP + "," +
                  data.altitude + "," +
                  data.WGS84_ellipsoid + "," +
                  data.DGPS_reference + "," +
                  data.checkSum + "," +
                  sensors.getTempCByIndex(0) + "," +
                  sensors.getTempCByIndex(0) + "," +
                  sensors.getTempCByIndex(1) + "," +
                  sensors.getTempCByIndex(1) + "," +
                  sensors.getTempCByIndex(2) + "," +
                  sensors.getTempCByIndex(2) + "," +
                  "1" + "," +
                  delta
                );
                openDoor();
              } else {
                SDwrite(
                  data.type + "," +
                  data.time + "," +
                  data.lat + "," +
                  data.lng + "," +
                  data.quality + "," +
                  data.numOfSatellites + "," +
                  data.HDOP + "," +
                  data.altitude + "," +
                  data.WGS84_ellipsoid + "," +
                  data.DGPS_reference + "," +
                  data.checkSum + "," +
                  sensors.getTempCByIndex(0) + "," +
                  sensors.getTempCByIndex(0) + "," +
                  sensors.getTempCByIndex(1) + "," +
                  sensors.getTempCByIndex(1) + "," +
                  sensors.getTempCByIndex(2) + "," +
                  sensors.getTempCByIndex(2) + "," +
                  "0" + "," +
                  delta
                );
                if(doorOpen){
                  closeDoor();
                }
              }
              break;
          }

          if (serialBuffer.length() == 0)
            break;
        }
      }

      serialBuffer = "";
    } else {
      // if this isn't the end of a line of serial data,
      // add it to the buffer to be parsed once the end is reached
      serialBuffer += serialChar;
    }
  }
}
