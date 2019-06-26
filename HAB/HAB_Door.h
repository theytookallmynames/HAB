#ifndef HAB_DOOR_H
#define HAB_DOOR_H

namespace HAB {
namespace Door {

// Max time to run the door motor in milliseconds before timing out.
// I'm not sure if this is necessary.
#define DOOR_MAX_WAIT 100000

enum DoorStatus {DOOR_OPEN, DOOR_CLOSED, DOOR_INTERMEDIATE};

void init();
DoorStatus getDoorStatus1();
DoorStatus getDoorStatus2();
void openDoor();
void closeDoor();

} // namespace Door
} // namespace HAB

#endif // HAB_DOOR_H
