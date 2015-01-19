/**
 * MRMain.cpp
 *
 * MR Arduino Main Program handled by the main Arduino sketch.
 *
 * Created: 1/5/2015 
 * Author: John Russo
 *
 * UPDATE LOG ================================================================================================
 * Update 1/8/2015 by John Russo: Serial communication with GS
 * Update 1/17/2015 by John Russo: Serial communication with CR
 * ===========================================================================================================
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
	
	///Motor
	//stepperMotor.initStepperMotor(200,8,7);
	//stepperMotor.setSpeed(4);
	//stepperMotor.enableStepping();
	
	//setup serial port
	Serial.begin(MR_GS_BAUD);
	Serial3.begin(MR_CR_BAUD);
	gsInputStringComplete = false;
	gsInputString = "";
	crInputStringComplete = false;
	crInputString = "";
	inChar = '0';
}

/** 
 * Main loop function for Mr Arduino
 * 
 */
void MRMain::loop()
{
	
	while (Serial.available() > 0) {
		// get the new byte
		inChar = (char)Serial.read();
		// add it to the inputString
		gsInputString += inChar;
		// if the incoming character is a newline, set a flag (end of command)
		if (inChar == '\n') {
			gsInputStringComplete = true;
		}
	}
	delay(250); // Doesn't work without this. My guess is it gives the serial buffer time to fill if the message isn't complete.
	
	if(gsInputStringComplete){
		//if its a command	
		if(gsInputString[0] == '$'){
			TIMSK3 |= (0 << TOIE3); //disable overflow interrupt
			parseCommand();		
			TIMSK3 |= (1 << TOIE3); //enable overflow interrupt
			gsInputString = "";
			gsInputStringComplete = false;
		}
	}
	

}
/**
 * Uses a switch statement to determine the type of command and proceeds to call the corresponding process function.                                                     
 */
void MRMain::parseCommand(){
	char commandType;
	commandType = gsInputString[1];
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
	
		blinkLED(5);
		Serial.print("$DP\n");
		Serial.flush();
}
	
/**
 * Blinks and LED 4 times and sends the image acknowledgment to the GS                                                    
 */
void MRMain::processImageCommand(){
		boolean imageRelayed = false;
		char imageChar = 0;
		blinkLED(4);
		
		//Relay command to CR
		Serial3.print(gsInputString);
		Serial3.flush();
		
		//Wait for Image to be received. 
		while(!Serial3.available()){
			//Just keep waiting for now
		}
		
		//Stream serial data to GS until finished with image
		while(!imageRelayed){
			if(Serial3.available() > 0){
				imageChar = Serial3.read();
				if(imageChar == '\n'){
					imageRelayed = true;
				}
				Serial.print(imageChar);
			}
		}
}

/**
 * Blinks and LED 3 times and sends the rappel acknowledgment to the GS                                                    
 */
void MRMain::processRappelCommand(){

		//LED Verification (make this a function)
		blinkLED(3);
		//Send Command to CR and wait for acknowledgment 
		Serial3.print(gsInputString);
		
		while(!Serial3.available()){
			//Wait here
		}
		delay(250); //Delay so serial buffer can fill
		//receive acknowledgment
		while(Serial3.available() > 0){
			// get the new byte:
			inChar = (char)Serial3.read();
			// add it to the inputString:
			crInputString += inChar;
			// if the incoming character is a newline, set a flag
			// so the main loop can do something about it:
			if (inChar == '\n') {
				crInputStringComplete = true;
			}
		}
		
		//Send acknowledgment to GS
		Serial.print(crInputString);
		Serial.flush();
}

/**
 * Blinks and LED 2 times and sends the status acknowledgment to the GS                                                    
 */
void MRMain::processStatusRequest(){
	
		blinkLED(2);
		Serial.print("$SP\n");
		Serial.flush();
}


/**
 * Blink LED n number of times.                                                    
 */
void MRMain::blinkLED(int num){
	delay(500);
	for(int i = 0;i<num;i++){
		digitalWrite(ALIVE_LED_PIN,LOW);
		delay(250);
		digitalWrite(ALIVE_LED_PIN,HIGH);
		delay(250);
		digitalWrite(ALIVE_LED_PIN,LOW);
		delay(250);
	}
}

int MRMain::readBatteryVoltage(){
	// Need battery circuit 
	int result = analogRead(BATTERY_PIN);
	return result/BATTERY_INPUT_RESOLUTION; //volt/(volts/volt)
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
	
	//stepperMotor.OCR1A_ISR();
}

MRMain mrMain;

