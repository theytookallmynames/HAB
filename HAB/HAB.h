#ifndef HAB_H
#define HAB_H

/**
 * Pins
 */
#define ONE_WIRE_BUS_PIN 14
#define SD_CARD_PIN 4
#define MOTOR_ONE_PIN_1 A4
#define MOTOR_ONE_PIN_2 12
#define MOTOR_TWO_PIN_1 A5
#define MOTOR_TWO_PIN_2 11
#define SERIAL_RECEIVE_PIN 10
#define SERIAL_TRANSMIT_PIN 9

/**
 * Other constants
 */

// Altitude in km at which the payload door should open/close
#define MOTOR_TRIGGER_ALTITUDE 46

// The base name for the log file.
// Each log file will be appended with an incrementing number.
#define LOG_FILE_BASE_NAME "GPSLOG"

#endif
