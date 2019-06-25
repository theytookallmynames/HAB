#include "HAB_Door.h"
#include "HAB_Logging.h"

namespace HAB {
namespace Door {

bool isOpening = false;
bool isClosing = false;


void init() {
  // The door switch pins are in pullup mode, and are tied to ground when the switches close.
  pinMode(DOOR_1_SWITCH_FRONT_PIN, INPUT_PULLUP); //INPUT_PULLUP sets the switch to high.
  pinMode(DOOR_1_SWITCH_REAR_PIN, INPUT_PULLUP);
  pinMode(DOOR_2_SWITCH_FRONT_PIN, INPUT_PULLUP);
  pinMode(DOOR_2_SWITCH_REAR_PIN, INPUT_PULLUP);

  
  //TODO: Remove all references to the MOTOR_DRIVER_ENABLE_PIN since the MOTOR_DRIVER_ENABLE_PIN is connected directly to the 5V line. 
  pinMode(MOTOR_1_DRIVER_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_1_DRIVER_REVERSE_PIN, OUTPUT);
  pinMode(MOTOR_2_DRIVER_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_2_DRIVER_REVERSE_PIN, OUTPUT);

  openDoor();
  closeDoor();

  Logging::logSystemData("Door initialized");
}

/**
 * The door can be in one of three states: open, intermediate or closed.
 *
 * The door is considered closed if the front limit switches are in the LOW state and the
 * rear limit switches are in the HIGH state meaning that the front limit switches are closed.
 * 
 * The door is considered intermediate if BOTH the front and rear limit switches are in the HIGH
 * state, meaning that none of the switches are closed.
 * 
 * The door is considered open if the front limit switches are in the HIGH state and the
 * rear limit switches are in the LOW state meaning that the rear limit switches are closed.
 *
 */
 
DoorStatus getDoorStatus1() {
  int frontDoorStatus = digitalRead(DOOR_1_SWITCH_FRONT_PIN);
  int rearDoorStatus = digitalRead(DOOR_1_SWITCH_REAR_PIN);

  if(frontDoorStatus == HIGH && rearDoorStatus == HIGH)
  {
    return DOOR_INTERMEDIATE;
  }

  if (frontDoorStatus == LOW) {
    return DOOR_CLOSED;
  }

  return DOOR_OPEN;

  
}

DoorStatus getDoorStatus2() {
  int frontDoorStatus = digitalRead(DOOR_2_SWITCH_FRONT_PIN);
  int rearDoorStatus = digitalRead(DOOR_2_SWITCH_REAR_PIN);

  if(frontDoorStatus == HIGH && rearDoorStatus == HIGH)
  {
    return DOOR_INTERMEDIATE;
  }

  if (frontDoorStatus == LOW) {
    return DOOR_CLOSED;
  }
  
   return DOOR_OPEN;
}


void stopMotor() {
  Logging::logSystemData("Stopping all motors");
  digitalWrite(MOTOR_1_DRIVER_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_1_DRIVER_REVERSE_PIN, LOW);
  digitalWrite(MOTOR_2_DRIVER_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_2_DRIVER_REVERSE_PIN, LOW); 
}

/**
 * Run the door motor until the door is determined to be opened.
 * Times out after DOOR_MAX_WAIT ms.
 *
 */
 
void openDoor() { //The doors open one at a time
  bool isOpening = true;
  uint32_t startTime;

  if (getDoorStatus1() == DOOR_OPEN && getDoorStatus2() == DOOR_OPEN) {
    return;
  }

  //Open door 1
  Logging::logSystemData("Opening Door 1");
  startTime = millis();
  while (getDoorStatus1() != DOOR_OPEN  && isOpening) {
    digitalWrite(MOTOR_1_DRIVER_FORWARD_PIN, HIGH);
    digitalWrite(MOTOR_1_DRIVER_REVERSE_PIN, LOW);
    
    if (millis() - startTime > DOOR_MAX_WAIT) {
      Logging::logSystemData(
        "openDoor() for door 1 timed out after " +
        String(DOOR_MAX_WAIT) +
        " ms. Aborting."
       );
       isOpening = false;
    }
  }
  Logging::logSystemData("Door 1 finished opening");
  stopMotor();
  

  //Open door 2
  isOpening = true;
  Logging::logSystemData("Opening Door 2");
  startTime = millis();
  while (getDoorStatus2() != DOOR_OPEN && isOpening){
    digitalWrite(MOTOR_2_DRIVER_FORWARD_PIN, HIGH);
    digitalWrite(MOTOR_2_DRIVER_REVERSE_PIN, LOW);
    
    if (millis() - startTime > DOOR_MAX_WAIT) {
      Logging::logSystemData(
        "openDoor() for door 2 timed out after " +
        String(DOOR_MAX_WAIT) +
        " ms. Aborting."
      );

      
      isOpening = false;
    }
  }
  Logging::logSystemData("Door 2 finished opening");
  stopMotor();
}

/**
 * Run the door motor until the door is determined to be closed.
 * Times out after DOOR_MAX_WAIT ms.
 *
 */
void closeDoor() { //Doors close one at a time
  bool isClosing = true;
  uint32_t startTime;
  
  if (getDoorStatus1() == DOOR_CLOSED && getDoorStatus2() == DOOR_CLOSED) {
    return;
  }

  //Close Door 1
  Logging::logSystemData("Closing Door 1");
  startTime = millis();
  while (getDoorStatus1() != DOOR_CLOSED && isClosing) {
    digitalWrite(MOTOR_1_DRIVER_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_1_DRIVER_REVERSE_PIN, HIGH);

    //Time Out for Door 1 in event of the switch failing.
    
    if (millis() - startTime > DOOR_MAX_WAIT) {
      Logging::logSystemData(
        "closeDoor() for door 1 timed out after " +
        String(DOOR_MAX_WAIT) +
        " ms. Aborting."
       );
       isClosing = false;
    }

  }
  Logging::logSystemData("Door 1 finished closing");
  stopMotor();

   //Close Door 2
   isClosing = true;
   Logging::logSystemData("Closing Door 2");
   startTime = millis();
   while (getDoorStatus2() != DOOR_CLOSED && isClosing) {
    digitalWrite(MOTOR_2_DRIVER_FORWARD_PIN, LOW);
    digitalWrite(MOTOR_2_DRIVER_REVERSE_PIN, HIGH);

    //Time Out for door 2

    if (millis() - startTime > DOOR_MAX_WAIT) {
      Logging::logSystemData(
        "closeDoor() for door 2 timed out after " +
        String(DOOR_MAX_WAIT) +
        " ms. Aborting."
       );
       isClosing = false;
     }
    }
    Logging::logSystemData("Door 2 finished closing");
    stopMotor();
  
}

} // namespace Door
} // namespace HAB
