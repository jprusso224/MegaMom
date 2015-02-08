// mrconstants.h

#ifndef _MRCONSTANTS_h
#define _MRCONSTANTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/*MOTOR*/
#define GEAR_RATIO 77//15
#define MAXSPEED 115//RPM
#define RAPPEL_ANGULAR_SPEED 5// 700 //mrad/s
#define CCW  1
#define CW 0
#define GAIN  5//Need to determine(may not need to be a global)
#define SPOOL_RADIUS 3 //cm

/*COMMUNICATION*/
#define MR_CR_BAUD 115200
#define MR_GS_BAUD 115200

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

#endif

