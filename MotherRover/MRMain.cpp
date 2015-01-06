/**
 * MRMain.cpp
 *
 * MR Arduino Main Program handled by the main Arduino sketch.
 *
 * Created: 1/5/2015 
 * Author: John Russo
 * Last Updated: 1/6/2015 by John Russo
 */ 

#include "MRMain.h"
#include "mrconstants.h"

/** 
 * Initialize function for the MR Arduino
 * As of now it is just testing the stepper motor class
 */
void MRMain::setup()
{
	//Setup alive LED
	pinMode(ALIVE_LED_PIN,OUTPUT);
	TCCR3A = 0;//set timer control registers to zero
	TCCR3B = 0;//same as above
	TCNT3  = 0;//initialize counter value to 0
	TCCR3B |= (1 << CS32); //prescaler 256
	TIMSK3 |= (1 << TOIE3); //enable overflow interrupt
	
	//stepper motor test
    stepperMotor.initStepperMotor(200,5,4); 
}

/** 
 * Main loop function for Mr Arduino
 * As of now it is just testing the stepper motor class
 */
void MRMain::loop()
{
	stepperMotor.setSpeed(4); //revolve 4 times per minute
	stepperMotor.enableStepping();
	delay(10000); // do it for 10 seconds
	stepperMotor.disableStepping();
}

ISR(TIMER3_OVF_vect){
	digitalWrite(ALIVE_LED_PIN,digitalRead(ALIVE_LED_PIN)^1);
}

/**
 * Interrupt service routine for timer 1 compare A interrupt
 *
 * Calls the interrupt handler function used to physically drive the stepper motor                                                     
 */
ISR(TIMER1_COMPA_vect){
	stepperMotor.OCR1A_ISR();
}

MRMain mrMain;

