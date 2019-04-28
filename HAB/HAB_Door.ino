#include "HAB_Door.h"
#include "HAB_Logging.h"

namespace HAB {
namespace Door {

bool isOpening = false;
bool isClosing = false;

void init() {
  // The door switch pins are in pullup mode, and are tied to ground when the switches close.
  pinMode(DOOR_SWITCH_FRONT_PIN, INPUT_PULLUP);
  pinMode(DOOR_SWITCH_REAR_PIN, INPUT_PULLUP);

  pinMode(MOTOR_DRIVER_ENABLE_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_DRIVER_REVERSE_PIN, OUTPUT);

  Logging::logSystemData("Door initialized");
}

/**
 * The door can be in one of three states: open, closed, or indeterminate.
 *
 * The door is considered closed if BOTH the front and rear limit switches are in the LOW state,
 * meaning that both switches are closed.
 *
 * The door is considered open if BOTH the front and rear limit switches are in the HIGH state,
 * meaning that both switches are open.
 *
 * The door is considered to be in an indeterminate state, meaning that one of the two limit
 * switches is open. This state indicates that the door is currently opening or closing.
 *
 * TODO: I think the indeterminate state can probably be removed (it's also not currently implemented).
 */
DoorStatus getDoorStatus() {
  int frontDoorStatus = digitalRead(DOOR_SWITCH_FRONT_PIN);
  int rearDoorStatus = digitalRead(DOOR_SWITCH_REAR_PIN);

  if (frontDoorStatus == LOW && rearDoorStatus == LOW) {
    return DOOR_CLOSED;
  }

  return DOOR_OPEN;
}

// TODO:
// https://learn.adafruit.com/adafruit-arduino-lesson-15-dc-motor-reversing/arduino-code
// void runMotor(const char* direction) {
//   switch (direction) {
//     case "open":
//       ...
//     case "close":
//       ...
//   }

//   /* analogWrite(MOTOR_DRIVER_ENABLE_PIN, DOOR_SPEED);
//   digitalWrite(MOTOR_DRIVER_FORWARD_PIN, HIGH);
//   digitalWrite(MOTOR_DRIVER_REVERSE_PIN, LOW);
//   delay(1000);
//   digitalWrite(MOTOR_DRIVER_FORWARD_PIN, LOW);
//   digitalWrite(MOTOR_DRIVER_REVERSE_PIN, HIGH);
//   delay(1000); */
// }

void stopMotor() {
  digitalWrite(MOTOR_DRIVER_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_DRIVER_REVERSE_PIN, LOW);
}

/**
 * Run the door motor until the door is determined to be opened.
 * Times out after DOOR_MAX_WAIT ms.
 */
void openDoor() {
  if (isOpening) {
    return;
  }

  isOpening = true;
  uint32_t startTime = millis();

  while (getDoorStatus() != DOOR_OPEN) {
    // TODO: run the motor

    if (millis() - startTime > DOOR_MAX_WAIT) {
      Logging::logSystemData(
        "openDoor() timed out after " +
        String(DOOR_MAX_WAIT) +
        " ms. Aborting."
      );
      break;
    }
  }

  isOpening = false;
}

/**
 * Run the door motor until the door is determined to be closed.
 * Times out after DOOR_MAX_WAIT ms.
 */
void closeDoor() {
  if (isClosing) {
    return;
  }

  isClosing = true;
  uint32_t startTime = millis();

  while (getDoorStatus() == DOOR_OPEN) {
    // TODO: run the motor

    if (millis() - startTime > DOOR_MAX_WAIT) {
      Logging::logSystemData(
        "closeDoor() timed out after " +
        String(DOOR_MAX_WAIT) +
        " ms. Aborting."
      );
      break;
    }
  }

  isClosing = false;
}

} // namespace Door
} // namespace HAB
