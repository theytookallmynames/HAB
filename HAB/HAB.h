#ifndef HAB_H
#define HAB_H

namespace HAB {

/**
 * With this set to 1, system initialization will wait until the serial monitor is connected
 * to begin operation. Set to 0 or comment it out prior to launch.
 */
#define HAB_DEVELOPMENT 1

/**
 * Pins
 */
#define SD_CARD_PIN 4
#define SERIAL_RECEIVE_PIN 10
#define SERIAL_TRANSMIT_PIN 9
#define PRESSURE_SENSOR_PIN 2

// Thermistor pins
#define TEMPERATURE_ONBOARD_PIN A0
#define TEMPERATURE_OUTDOOR_TOP_PIN A1
#define TEMPERATURE_OUTDOOR_BOTTOM_PIN A2

#define SENSOR_STATUS_LED_PIN A4
#define GPS_STATUS_LED_PIN A3
#define SD_STATUS_LED_PIN 23

#define MOTOR_DRIVER_ENABLE_PIN 11
#define MOTOR_1_DRIVER_FORWARD_PIN 10
#define MOTOR_1_DRIVER_REVERSE_PIN 9
#define MOTOR_2_DRIVER_FORWARD_PIN 13
#define MOTOR_2_DRIVER_REVERSE_PIN 12

#define DOOR_1_SWITCH_FRONT_PIN 11
#define DOOR_1_SWITCH_REAR_PIN A5
#define DOOR_2_SWITCH_FRONT_PIN 6
#define DOOR_2_SWITCH_REAR_PIN 5

/**
 * Other constants
 */

// Altitude in millimiters at which the payload door should open/close
#define MOTOR_OPEN_TRIGGER_ALTITUDE 18288000
#define MOTOR_CLOSE_TRIGGER_ALTITUDE 16764000

} // namespace HAB

#endif
