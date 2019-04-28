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
#define ONE_WIRE_BUS_PIN A0
#define SD_CARD_PIN 4
#define SERIAL_RECEIVE_PIN 10
#define SERIAL_TRANSMIT_PIN 9
#define PRESSURE_SENSOR_PIN 2
#define DOOR_SWITCH_FRONT_PIN A1
#define DOOR_SWITCH_REAR_PIN A2
#define TEMPERATURE_ONBOARD_PIN A7
#define SENSOR_STATUS_LED_PIN A3
#define GPS_STATUS_LED_PIN A4
#define SD_STATUS_LED_PIN A5
#define MOTOR_DRIVER_ENABLE_PIN 11
#define MOTOR_DRIVER_FORWARD_PIN 10
#define MOTOR_DRIVER_REVERSE_PIN 9

/**
 * Other constants
 */

// Altitude in km at which the payload door should open/close
#define MOTOR_TRIGGER_ALTITUDE 46

} // namespace HAB

#endif
