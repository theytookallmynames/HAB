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

#include "HAB.h"
#include "GPS.h"

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS_PIN);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// Used for tracking the change in altitude to determine when to open/close the door
float delta_y = 0.0;
float y_1 = 0.0;
float y_2 = 0.0;

void SDwrite();
void openDoor();
void closeDoor();

// current open state of the door. true if open, false if closed.
bool doorOpen = false;

// receive pin = 10, transmit pin = 9
SoftwareSerial serialDataBus(SERIAL_RECEIVE_PIN, SERIAL_TRANSMIT_PIN);

// data from the GPS serial input is read into here until it's ready to be parsed
String gpsDataBuffer = "";

File logfile;
String logFileName;

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
  pinMode(SD_CARD_PIN, OUTPUT);
  pinMode(MOTOR_ONE_PIN_1, OUTPUT);
  pinMode(MOTOR_ONE_PIN_2, OUTPUT);
  pinMode(MOTOR_TWO_PIN_1, OUTPUT);
  pinMode(MOTOR_TWO_PIN_2, OUTPUT);

  if (!SD.begin(SD_CARD_PIN)) {
    Serial.println("Card failed, or not present");
    return;
  }

  Serial.println("card initialized.");

  for (uint8_t i = 0; i < 1000; i++) {
    if (!SD.exists(String(LOG_FILE_BASE_NAME) + i + ".csv")) {
      logFileName = String(LOG_FILE_BASE_NAME) + i + ".csv";
      break;
    }
  }

  serialDataBus.begin(4800);
  Serial.println("Ready!");
}

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
  digitalWrite(MOTOR_ONE_PIN_1, HIGH);
  digitalWrite(MOTOR_ONE_PIN_2, LOW);
  digitalWrite(MOTOR_TWO_PIN_1, HIGH);
  digitalWrite(MOTOR_TWO_PIN_2, LOW);
}

void closeDoor() {
  doorOpen = false;
  digitalWrite(MOTOR_ONE_PIN_1, LOW);
  digitalWrite(MOTOR_ONE_PIN_2, HIGH);
  digitalWrite(MOTOR_TWO_PIN_1, LOW);
  digitalWrite(MOTOR_TWO_PIN_2, HIGH);
}

void loop() {
  if (serialDataBus.available()) {
    char serialChar = (char) serialDataBus.read();

    // if we've reached the end of a line of serial input, parse it.
    if (serialChar == '\n') {
      if (isGPSData(gpsDataBuffer)) {
        // the GPGGA sentence contains the altitude data
        if (isGPGGA(gpsDataBuffer)) {
          y_2 = getAltitude(gpsDataBuffer);
          delta_y = y_2 - y_1;
          y_1 = y_2;

          if (y_1 > MOTOR_TRIGGER_ALTITUDE && !doorOpen) {
            openDoor();
          } else if (y_1 < MOTOR_TRIGGER_ALTITUDE && doorOpen) {
            closeDoor();
          }
        }

        SDwrite(gpsDataBuffer);

        SDwrite(
          String("$HAB_TEMP,") +
          sensors.getTempCByIndex(0) + "," +
          sensors.getTempCByIndex(0) + "," +
          sensors.getTempCByIndex(1) + "," +
          sensors.getTempCByIndex(1) + "," +
          sensors.getTempCByIndex(2) + "," +
          sensors.getTempCByIndex(2) + ","
        );

        SDwrite("$HAB_DOOR," + doorOpen);

        // TODO: write out pressure data
        // SDwrite("$HAB_PRESSURE,");
      }

      // clear the data buffer for the next line of GPS data
      gpsDataBuffer = "";
    } else {
      // if this isn't the end of a line of serial data,
      // add it to the buffer to be parsed once the end is reached
      gpsDataBuffer += serialChar;
    }
  }
}
