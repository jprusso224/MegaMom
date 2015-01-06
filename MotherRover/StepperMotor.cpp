/**
 * StepperMotor.cpp
 *
 * MR Arduino Main Program handled by the main Arduino sketch.
 *
 * Created: 1/5/2015 
 * Author: John Russo
 * Last Updated: 1/6/2015 by John Russo
 */ 

#include "StepperMotor.h"

void StepperMotor::OCR1A_ISR(){
	if(digitalRead(STEPPIN)){
		if(direction){
			stepCount++; // increment if unspooling
		}else{
			stepCount--; // decrement if spooling
		}
	}
}

void StepperMotor::initStepperMotor(int stepsPerRev, int dirPin, int enPin){
	
    stepsPerRevolution = stepsPerRev;
	directionPin = dirPin;
	enablePin = enPin;
	
	TCCR1A = 0;//set timer control registers to zero
	TCCR1B = 0;//same as above
	TCNT1  = 0;//initialize counter value to 0
	
	// set compare register to 0 initially
	OCR1A = 0;
	// turn on CTC mode (no prescaler for timer)
	TCCR1B |= (1 << WGM12);
	// initially the CTC interrupt must be disabled
	TIMSK1 |= (0 << OCIE1A);
	// set the interrupt to toggle OCA1(located on pin 11)
	pinMode(11,OUTPUT);
	TCCR1A |= (1 << COM1A0);
	
	direction = 1; //subject to change 
	enabled = false; //initially off
	stepCount = 0;
}

void StepperMotor::setSpeed(int RPM){
	//update stored value
	speed = RPM;
	//Convert rpm into a value for OCR1A
	int stepsPerSec = RPM*stepsPerRevolution/60;
	OCR1A = (16*10^16)/(stepsPerSec) + 1;
}

void StepperMotor::setDirection(int dir){
	direction = dir;
	digitalWrite(directionPin,direction);
}

int StepperMotor::getDirection(){
	return direction;
}

void StepperMotor::enableStepping(){
	enabled = true;
	TIMSK1 |= (1 << OCIE1A); //enable interrupts
	digitalWrite(enablePin,enabled);
}

void StepperMotor::disableStepping(){
	enabled = false;
	TIMSK1 |= (0 << OCIE1A); //disable interrupts
	digitalWrite(enablePin,enabled);
}

void StepperMotor::incStepCount(){
	stepCount++;
}

void StepperMotor::decStepCount(){
	stepCount--;
}

int StepperMotor::getStepCount(){
	return stepCount;
}

StepperMotor stepperMotor;


