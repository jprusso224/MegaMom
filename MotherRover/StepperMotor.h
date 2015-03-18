/**
 * StepperMotor.h
 *
 * MR Arduino Main Program handled by the main Arduino sketch.
 *
 * Created: 1/5/2015 
 * Author: John Russo
 */ 

#ifndef _STEPPERMOTOR_h
#define _STEPPERMOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "mrconstants.h"



class StepperMotor
{
 public:
	
	/**
	 * Setup function for the stepper motor. 
	 *
	 * Initializes timers, interrupts, and variables required for stepper motor use.
	 *
	 * @param stepsPerRev steps per revolution of the stepper motor
	 * @param dirPin pin on Arduino board to be used for controlling motor direction
	 * @param enPin pin on Arduino board to be used for enabling the motor                                            
	 */
	void initStepperMotor(int stepsPerRev,int dirPin, int enPin);
	
	/**
	 * Sets the speed of the stepper motor
	 * 
	 * @param RPM desired speed in millirads/sec                                                  
	 */
	void setSpeed(int angularSpeed);
	
	void setOCR1A(int freq);
	
	/**
	 * enable motor stepping functionality                                                    
	 */
	void enableStepping();
	
	/**
	 * disable motor stepping functionality                                                    
	 */
	void disableStepping();
	
	/**
	 * set direction of stepper motor
	 *
	 * @param dir desired motor direction. 0 or 1.                                                    
	 */
	void setDirection(int dir);
	
	/**
	 * gets direction of stepper motor
	 * 
	 * @return current motor direction                                                    
	 */
	int getDirection();
	
	/**
	 * increment step count                                                    
	 */
	void incStepCount();
	
	/**
	 * decrement step count                                                    
	 */
	void decStepCount();
	
	/**
	 * get step count
	 *
	 * @return current step count                                                    
	 */
	int getStepCount();
	
	/**
	 * interrupt handler used to drive stepper motor                                                    
	 */
	void OCR1A_ISR();
	
 private:
	 int stepsPerRevolution;
	 int stepsPerRad;
	 int direction;
	 int speed;
     boolean enabled;
	 int directionPin;
	 int enablePin;
	 volatile int stepCount;
};

extern StepperMotor stepperMotor;

#endif

