/*
 * MRMain.h
 *
 * Created: 1/5/2015 
 * Author: John Russo
 * Last Updated: 1/6/2015 by John Russo
 */ 

#ifndef _MRMAIN_h
#define _MRMAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "StepperMotor.h"

class MRMain
{
 private:


 public:
    /** 
     * Initialize function for the MR Arduino
     */
	void setup();
	
	/**
	 * Main loop function for Mr Arduino
	 */
	void loop();
};

extern MRMain mrMain;

#endif

