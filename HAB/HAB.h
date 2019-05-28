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
#define TEMPERATURE_ONBOARD_PIN A7
#define TEMPERATURE_OUTDOOR_TOP_PIN 14
#define TEMPERATURE_OUTDOOR_BOTTOM_PIN 16

#define SENSOR_STATUS_LED_PIN 22
#define GPS_STATUS_LED_PIN 23
#define SD_STATUS_LED_PIN 15

#define MOTOR_DRIVER_ENABLE_PIN 11
#define MOTOR_1_DRIVER_FORWARD_PIN 10
#define MOTOR_1_DRIVER_REVERSE_PIN 9
#define MOTOR_2_DRIVER_FORWARD_PIN 13
#define MOTOR_2_DRIVER_REVERSE_PIN 12

#define DOOR_1_SWITCH_FRONT_PIN A0
#define DOOR_1_SWITCH_REAR_PIN A1
#define DOOR_2_SWITCH_FRONT_PIN A2
#define DOOR_2_SWITCH_REAR_PIN A3

/**
 * Other constants
 */

// Altitude in km at which the payload door should open/close
#define MOTOR_TRIGGER_ALTITUDE 46

} // namespace HAB

#endif
