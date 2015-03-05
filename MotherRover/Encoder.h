// Encoder.h

#ifndef _ENCODER_h
#define _ENCODER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "mrconstants.h"

class Encoder
{
 protected:
	int _interruptNumber;
	int _resolution;
	long pulseCount;
	int _type;
	int _side;
	int _ePin;
	int _dirPin;
	
 public:
	void initEncoder(int interruptNumber, int resolution, int dirPin,int ePin);
	int getDistanceTraveled();
	void encoderISR();
	int getPulseCount();
};

#endif

