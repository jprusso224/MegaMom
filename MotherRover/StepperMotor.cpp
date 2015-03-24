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
	/*if(digitalRead(STEPPIN)){
		if(direction){
			stepCount++; // increment if unspooling
		}else{
			stepCount--; // decrement if spooling
		}
	}*/
}

void StepperMotor::initStepperMotor(int stepsPerRev, int dirPin, int enPin){
	
    stepsPerRevolution = stepsPerRev;
	directionPin = dirPin;
	enablePin = enPin;
	
	// Convert to steps/rad so we don't have to deal with floating points in the rappel loop.
	stepsPerRad = int(stepsPerRevolution/(2*PI));
	
	TCCR1A = 0;//set timer control registers to zero
	TCCR1B = 0;//same as above
	TCNT1  = 0;//initialize counter value to 0
	
	// set compare register to 0 initially
	OCR1A = MINSPEED;
	// turn on CTC mode (no prescaler for timer)
	TCCR1B |= (1 << WGM12);
	TCCR1B |= (1 << CS11);
	
	// initially the CTC interrupt must be disabled
	//TIMSK1 |= (1 << OCIE1A);
	// set the interrupt to toggle OCA1(located on pin 11)
	pinMode(11,OUTPUT);
	pinMode(dirPin,OUTPUT);
	pinMode(enPin,OUTPUT);
	TCCR1A |= (0 << COM1A0);
	
	digitalWrite(enPin,LOW);
	digitalWrite(11,LOW);

	
	direction = 1; //subject to change 
	enabled = false; //initially off
	stepCount = 0;
}

void StepperMotor::setSpeed(int angularSpeed){
	//avoid divide by zero (may have to change this)
	if(angularSpeed == 0){
		disableStepping();
	}
	
	//update stored value
	speed = int(angularSpeed*GEAR_RATIO); // mrads/sec
	//Convert rpm into a value for OCR1A
	
	long stepsPerSec = long(angularSpeed*GEAR_RATIO*stepsPerRad)/1000;
	long longOCR1A = CLOCKSPEED/1024/stepsPerSec/2;
	OCR1A = int(longOCR1A);
}

void StepperMotor::setOCR1A(int freq){
	
		OCR1A = (int)(CLOCKSPEED/16.0/float(freq));
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
//	TIMSK1 |= (1 << OCIE1A); //enable interrupts
	TCCR1A |= (1 << COM1A0);
	digitalWrite(enablePin,LOW);
}

void StepperMotor::disableStepping(){
	enabled = false;
//	TIMSK1 |= (0 << OCIE1A); //disable interrupts
	TCCR1A &= ~(1 << COM1A0);
	digitalWrite(enablePin,LOW);
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


