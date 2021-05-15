# AER201 - AutoCone12

The goal of this project is to create a prototype autonomous robot that can preform a series of complex functions related to the deployment of traffic cones.

## Microcontrollers

* PIC18F4620-I/P
    * PIC DevBugger
    * HD44780-constrolled LCD Display (4x16 characters)
    * 4x4 matrix keypad
* Arduino Nano

## Hardware
* 4 DC motors - wheels
* 1 Servo Motor - cone positioning
* 2 Stepper Motors - cone deployment

## Functionality

### PIC

The job was the PIC was to act as a user interface via a keypad and LCD display. This allowed the user to input the number of cones they load into the machine, put the robot into position, and then start the robot. While the robot completed the task, the LCD display would log useful information such as when a cone was being deployed, the total number of cones deployed and that remain. When the task was completed, the LCD display would show summary information of the task such as the number of cones deployed and how long the task took to complete.

### Arduino Nano

The job of the arduino was to control the hardware components and the overall control flow of the robot during the task. This includes all of the logic related to detecting the correct pattern, moving the robot into the correct position, moving the deployment mechanism into the correct position, deploying the correct number of cones, and communicating this information back to the PIC microcontroller.
