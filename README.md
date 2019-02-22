# HAB
High altitude balloon project for Bergen's 2018 STEM program

## Setting up your development environment

1. Clone this repo.
2. Install the Arduino IDE: https://www.arduino.cc/en/main/software
3. Open the repo in the IDE
4. Follow [these instructions](https://learn.adafruit.com/adafruit-feather-32u4-basic-proto/arduino-ide-setup) to install the "Adafruit AVR Boards" package in the IDE, and then set the board for the sketch to "Adafruit Feather 32u4" under the Tools > Boards menu.

You may need to install some additional libraries. To do that, go to the Tools menu in the Arduino IDE, search for the missing libraries, and hit the install button.

## Building the code

In the Arduino IDE, hit the checkmark button at the top of the window (or select Verify/Compile in the Sketch menu). This will make sure the code compiles correctly and will give some information at the bottom of the screen such as how much storage the compiled binary will take up on the device, and how much dynamic memory will be available for the program after global variables are taken into account.

## Installing on the arduino

TODO

## Hardware

### Board

Adafruit Feather 32u4

https://learn.adafruit.com/adafruit-feather-32u4-bluefruit-le/overview

### Sensors

#### Temperature sensors (3x)

Onboard: Velleman VMA320 analog temperature sensor module. Connected to port A0 on board.

Inside payload container: Maxim DS18B20 digital thermometer. Connected to port A2 on board.

Outside payload container: Maxim DS18B20 digital thermometer. Connected to port A3 on board.

#### Pressure sensor (1x)

Honeywell SIP RN. Pin 3 connected to SDA, pin 4 connected to SCL.

#### GPS receiver (1x)

NaviSys GM-401

#### Door motor (1x)
