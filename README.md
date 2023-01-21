# shooter_test
VEX V5 Shooter Tester program

## Installation
Clone the repository, then open it in PROS and make/upload

## Usage
* Connect up to 4 V5 motors to any ports
* Start the program. If the screen is black, click on the far right of the screen (~1/2" from the edge). 
* It will auto-detect the connected motors and assign them letters A,B,C,D in order of their port number
* Configure the motors on the first tab
* Control motors on the second tab
* See power consumption and spin-up data on the third tab. Data is also printed to the PROS terminal.

## Configuration Tab
* Configuration may be changed at any time but some settings may not apply until the motor is turned off.
* Each motor may be configured by clicking the buttons under the motor. The icon and label will change to indicate the current configuration.
* Motor may be set as a leader or follower. By default all motors are leaders. If following, they will follow the motor to the left (lower letter). Followers inherit all settings except reverse direction, and should be used when two motors are physically geared together.
* Forward or Reverse may be set for each motor. The rest of the program assumes all motor speeds are positive.
* Gear ratio may be set to any value (36:1, 18:1, 6:1, corresponding to red, green, and blue cartridges)

## Operation Tab
* Clicking on the power button for each motor will turn it and all followers on and off
* The measured speed is displayed at all times (even when off), and will turn green when the speed is within 5% of the set speed
* The set speed may be changed with the up/down buttons for each motor separately
