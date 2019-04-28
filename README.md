# HAB
High altitude balloon project for Bergen's 2018 STEM program

## Setting up your development environment

1. Clone this repo.
2. Install the Arduino IDE: https://www.arduino.cc/en/main/software
3. Open the `HAB` directory in the repo in the IDE
4. Follow [these instructions](https://learn.adafruit.com/adafruit-feather-32u4-basic-proto/arduino-ide-setup) to install the "Adafruit AVR Boards" package in the IDE, and then set the board for the sketch to "Adafruit Feather M0" under the Tools > Boards menu.

You may need to install some additional libraries. To do that, go to the Tools menu in the Arduino IDE, search for the missing libraries, and hit the install button.

## Building the code

In the Arduino IDE, hit the checkmark button at the top of the window (or select Verify/Compile in the Sketch menu). This will make sure the code compiles correctly and will give some information at the bottom of the screen such as how much storage the compiled binary will take up on the device, and how much dynamic memory will be available for the program after global variables are taken into account.

## Installing on the arduino

In the Arduino IDE, press the button with the arrow (next to the button with the checkmark). If the code verifies properly, it will get uploaded to the Arduino and the Arduino will restart with the new code.

## Hardware

See the docs folder for datasheets.

### Board

Adafruit Feather M0 Adalogger

https://learn.adafruit.com/adafruit-feather-m0-adalogger/overview

### Sensors

#### Temperature sensor

Velleman VMA320 analog temperature sensor module

#### Pressure sensor

Honeywell HSCRNN1.6BA2A3

#### GPS receiver

Byonics GPS5HAOEM module
NaviSys GM-401 SiRFstarIV chip
