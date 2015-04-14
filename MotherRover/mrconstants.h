// mrconstants.h

#ifndef _MRCONSTANTS_h
#define _MRCONSTANTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/*MOTOR*/
#define GEAR_RATIO 77.0//15.3
#define MINSPEED 400//clockCycles
#define RAPPEL_ANGULAR_SPEED 7000// clockCycles //124
#define CCW  1
#define CW 0 //Mental Note: These are switched 
#define GAIN  5//Need to determine(may not need to be a global)
#define SPOOL_RADIUS 4.6 //cm
#define STEPS_PER_REV 1
#define MOTOR_DIR_PIN 10
#define MOTOR_EN_PIN 9


/*COMMUNICATION*/
#define MR_CR_BAUD 115200
#define MR_GS_BAUD 115200
// switch on XBee shield has to be in DLINE for programming and UART for execution
// UART maps serial from XBee to pins 0/1 and DLINE maps serial from XBee to ports 2/3

/*COMMAND STRINGS*/
#define GET_DEPTH "$R0\n"
#define ACKNOWLEDGE_RAPPEL "$RP\n"
#define ACKNOWLEDGE_DRIVE "$DP\n"

/*SERIAL DELAY CONSTANTS*/
#define RAPPEL_SERIAL_DELAY 25 //ms

/*GENERAL MR CONSTANTS*/
#define ALIVE_LED_PIN 13
#define BATTERY_PIN A0
#define BATTERY_INPUT_RESOLUTION 1 //Change this when Greg finishes circuit.
#define STEPPIN 11
#define CLOCKSPEED 16000000

/*ENCODER*/
#define ENCODER_PULSE_PIN 21
#define ENCODER_INT 2
#define ENCODER_RESOLUTION 300.0
#define ENCODER_DIR_PIN 30

/*Power Measurement*/
#define R1 12000
#define R2 5000
#define BATTERY_MEASUREMENT_PIN A0

#endif

