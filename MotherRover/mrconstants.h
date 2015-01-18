// mrconstants.h

#ifndef _MRCONSTANTS_h
#define _MRCONSTANTS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

/*COMMUNICATION*/
#define MR_CR_BAUD 115200
#define MR_GS_BAUD 115200

/*GENERAL MR CONSTANTS*/
#define ALIVE_LED_PIN 13
#define BATTERY_PIN A0
#define BATTERY_INPUT_RESOLUTION 1 //Change this when Greg finishes circuit.

#endif

