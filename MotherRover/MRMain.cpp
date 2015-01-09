/**
 * MRMain.cpp
 *
 * MR Arduino Main Program handled by the main Arduino sketch.
 *
 * Created: 1/5/2015 
 * Author: John Russo
 * Last Updated: 1/8/2015 by John Russo
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
	
	//setup serial port
	Serial.begin(9600);
	inputStringComplete = false;
	inputString = "";
	inChar = '0';
}

/** 
 * Main loop function for Mr Arduino
 * As of now it is just testing the stepper motor class
 */
void MRMain::loop()
{
	
	while (Serial.available() > 0) {
		// get the new byte:
		inChar = (char)Serial.read();
		// add it to the inputString:
		inputString += inChar;
		// if the incoming character is a newline, set a flag
		// so the main loop can do something about it:
		if (inChar == '\n') {
			inputStringComplete = true;
		}
	}
	delay(250); // Doesn't work without this. My guess is it gives the serial buffer time to fill if the message isn't complete.
	
	if(inputStringComplete){
		//if its a command	
		if(inputString[0] == '$'){
			TIMSK3 |= (0 << TOIE3); //disable overflow interrupt
			parseCommand();		
			TIMSK3 |= (1 << TOIE3); //enable overflow interrupt
			inputString = "";
			inputStringComplete = false;
		}
	}
	

}
/**
 * Uses a switch statement to determine the type of command and proceeds to call the corresponding process function.                                                     
 */
void MRMain::parseCommand(){
	char commandType;
	commandType = inputString[1];
	switch(commandType){
		case 'R':
			processRappelCommand();
			break;
		case 'I':
			processImageCommand();
			break;
		case 'D':
			processDriveCommand();
			break;
		default:
			processStatusRequest();
			break;
	}
}

/**
 * Blinks and LED 5 times and sends the drive acknowledgment to the GS                                                    
 */
void MRMain::processDriveCommand(){
	
		delay(500);
		for(int i = 0;i<5;i++){
			digitalWrite(ALIVE_LED_PIN,LOW);
			delay(250);
			digitalWrite(ALIVE_LED_PIN,HIGH);
			delay(250);
			digitalWrite(ALIVE_LED_PIN,LOW);
			delay(250);
		}
		Serial.print("$DP\n");
		Serial.flush();
}
	
/**
 * Blinks and LED 4 times and sends the image acknowledgment to the GS                                                    
 */
void MRMain::processImageCommand(){
	
		delay(500);
		for(int i = 0;i<4;i++){
			digitalWrite(ALIVE_LED_PIN,LOW);
			delay(250);
			digitalWrite(ALIVE_LED_PIN,HIGH);
			delay(250);
			digitalWrite(ALIVE_LED_PIN,LOW);
			delay(250);
		}
		Serial.print("$IP\n");
		Serial.flush();
}

/**
 * Blinks and LED 3 times and sends the rappel acknowledgment to the GS                                                    
 */
void MRMain::processRappelCommand(){

		delay(500);
		for(int i = 0;i<3;i++){
			digitalWrite(ALIVE_LED_PIN,LOW);
			delay(250);
			digitalWrite(ALIVE_LED_PIN,HIGH);
			delay(250);
			digitalWrite(ALIVE_LED_PIN,LOW);
			delay(250);
		}
		Serial.print("$RP\n");
		Serial.flush();
}

/**
 * Blinks and LED 2 times and sends the status acknowledgment to the GS                                                    
 */
void MRMain::processStatusRequest(){
	
		delay(500);
		for(int i = 0;i<2;i++){
			digitalWrite(ALIVE_LED_PIN,LOW);
			delay(250);
			digitalWrite(ALIVE_LED_PIN,HIGH);
			delay(250);
			digitalWrite(ALIVE_LED_PIN,LOW);
			delay(250);
		}
		Serial.print("$SP\n");
		Serial.flush();
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

