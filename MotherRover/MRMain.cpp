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
 * Update 2/2/2015 by John Russo: processRappelCommand() implemented. Still need gain. 
 * Update 3/13/15 by Casey Zahorik: Imaging debugging
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
	stepperMotor.initStepperMotor(STEPS_PER_REV,MOTOR_DIR_PIN,MOTOR_EN_PIN);
	stepperMotorEncoder.initEncoder(ENCODER_INT,ENCODER_RESOLUTION,ENCODER_DIR_PIN,ENCODER_PULSE_PIN);//Need to specify this!!!
	
	//Rappelling
	currentDepth = 0; //cm
	tetherLetOut = 0;
	
	//setup serial port
	Serial.begin(MR_GS_BAUD); // to GS
	Serial3.begin(MR_CR_BAUD); // to MRssssssss
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
	
	if(Serial.available() > 0){
		//noInterrupts();
	}
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
	//interrupts();
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
		//Serial.println("Command Recevied");
	commandType = gsInputString[1];
	switch(commandType){
		case 'R':
			if(gsInputString[2] == '0'){
				processRappelCommand();
			}else{
				processReturnCommand();
			}
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
	
		String targetString = "";
		int driveDistance = 0;
		long int startCount = 0L;
		long int targetCount = 0L;
		char fromCR = NULL;
		
		blinkLED(5);
	
		/*Determine whether turning or driving*/
		char driveType;
		driveType = gsInputString[2];
		
		switch (driveType)
		{
			case 'L':
				break;
			case 'R':
				break;
			case 'B':
				break;
			default:
				//Drive forwards
				targetString = gsInputString.substring(4,7);
				driveDistance = targetString.toInt();
				startCount = stepperMotor.getStepCount();
				
				targetCount = startCount + long((driveDistance*STEPS_PER_REV*GEAR_RATIO)/SPOOL_RADIUS/PI);
				
				stepperMotor.setSpeed(RAPPEL_ANGULAR_SPEED);
				stepperMotor.setDirection(CCW);
				stepperMotor.enableStepping();
				
				while(stepperMotor.getStepCount() < targetCount){
					//Just keep stepping
				}
				
				//Send command to CR
				Serial3.print(gsInputString);
				
				//Wait for acknowledgment
				while(!Serial3.available()){
							//Wait here for depth from CR
				}
				delay(RAPPEL_SERIAL_DELAY); //Delay so serial buffer can fill
				while(Serial3.available() > 0){
					fromCR = (char)Serial3.read();
					crInputString += fromCR;
					if (fromCR == '\n') {
						crInputStringComplete = true;
					    Serial.print(crInputString);
					    Serial.flush();
					}
				}
				break;
		}
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
		Serial.flush();
}

/**
 * Blinks and LED 3 times and sends the rappel acknowledgment to the GS                                                    
 */
void MRMain::processRappelCommand(){
		//Serial.println("Command Rappel!");
		//Declare local variables
		String targetString = "";
		int rappelDistance = 0;
		int targetDepth = 0;
		char fromCR = NULL;
		String crDepthString = "";
		int desiredSpeed = MINSPEED;
		int motorSpeed = MINSPEED;
		int controlError = 20; //cm
		long serialTime0 = 0L;
		long serialTime = 0L;
		int counter = 0;
		String acknowledge = "$R0P";
		int gsDebug = 0;
		
		//LED Verification (visual)
		//blinkLED(3);
		
		//Extract depth from command and set target
		targetString = gsInputString.substring(4,7);
		rappelDistance = targetString.toInt();
		if(gsInputString[3] == '-'){
			rappelDistance = -rappelDistance;
			stepperMotor.setDirection(CCW);
		}else{
			stepperMotor.setDirection(CW);
		}
		targetDepth = currentDepth + rappelDistance - 20;
		//Serial.println("No Motor!");
		//Set up the rest of stepper motor
		stepperMotor.setSpeed(motorSpeed);
		stepperMotor.enableStepping();
		//Serial.println("Motor!");
		// Enter rappelling loop
		Serial3.print(GET_DEPTH);
		while(!Serial3.available()){
			//
		}
		while(Serial3.available() > 0){
				fromCR = (char)Serial3.read();
				crInputString += fromCR;
				if (fromCR == '\n') {
					crInputStringComplete = true;
					
			}
		}
		
		crDepthString = crInputString.substring(3,6);
		currentDepth = crDepthString.toInt();
		Serial.println(currentDepth);
		
		while(currentDepth < targetDepth){
		//	Serial.println("Looping!");
			//Get depth from CR	
			
			TCCR1A &= ~(1 << COM1A0);
			Serial3.print(GET_DEPTH);
			TCCR1A |= (1 << COM1A0);
				
			serialTime0 = millis();
			counter = 0;
			while(!Serial3.available()){
				//Wait here for depth from CR
				serialTime = millis();
				if(serialTime - serialTime0 > 2000){
					Serial.print("Resending command to CR...\n");
					counter++;
					noInterrupts();
					Serial3.print(GET_DEPTH);
					interrupts();
					serialTime0 = millis();
					if(counter == 4){
						Serial.println("Failed to communicate with CR");
						acknowledge = "$R0F";
						break;
					}
				}		
			}
			if(counter < 4 ){
				//Serial.println("Distance received");
				delay(RAPPEL_SERIAL_DELAY); //Delay so serial buffer can fill
				if(Serial3.available() > 0){
				}
				while(Serial3.available() > 0){
					fromCR = (char)Serial3.read();
					crInputString += fromCR;
					if (fromCR == '\n') {
						crInputStringComplete = true;
					
					}
				}
				interrupts();
				//Extract CR depth from 
				crDepthString = crInputString.substring(3,6);
				crInputString = "";
				currentDepth = crDepthString.toInt();
				gsDebug = targetDepth - currentDepth;
				Serial.println(currentDepth);
		    
				//Check to see if you even need to set stepper motor
				if(currentDepth >= targetDepth){
					stepperMotor.setSpeed(MINSPEED);
					break;
				}
			
				//Set the stepper motor speed
				if(targetDepth - currentDepth  > controlError){
					stepperMotor.setOCR1A(RAPPEL_ANGULAR_SPEED); //constant speed
				}else{
					//desiredSpeed = ((targetDepth - currentDepth)*RAPPEL_ANGULAR_SPEED)/controlError;//cm/s
					//motorSpeed = desiredSpeed/SPOOL_RADIUS; //mrad/s
					desiredSpeed = MINSPEED - ((MINSPEED - RAPPEL_ANGULAR_SPEED)/controlError)*(targetDepth - currentDepth);
					stepperMotor.setOCR1A(desiredSpeed); //mrad/s
				}
			}else{
				break;
			}
		}
		//Disable stepper motor
		stepperMotor.disableStepping();   
		
		//Send Acknowledgment to GC
		Serial.println(acknowledge);
		Serial.flush();

}
		

/**
 * Uses the stepper motor to reel in the CR. Step count ensures all tether is reeled in.                                                    
 */
void MRMain::processReturnCommand(){
	//REEL IN THE CR
}

/**
 * Blinks and LED 2 times and sends the status acknowledgment to the GS                                                    
 */
void MRMain::processStatusRequest(){
	
		blinkLED(2);
		Serial.print("$SP\n");
		Serial.flush();
}

void MRMain::autoSpoolOut(int commandLength){
	int targetTetherOut = 0;
	targetTetherOut = tetherLetOut + commandLength;
	
	stepperMotor.setDirection(CW);
	stepperMotor.setSpeed(RAPPEL_ANGULAR_SPEED);
	stepperMotor.enableStepping();
	
	while(tetherLetOut < targetTetherOut){
		//Drive stepper motor
		tetherLetOut = stepperMotorEncoder.getDistanceTraveled();
	}
	
	stepperMotor.disableStepping();
}

void MRMain::autoReelIn(int commandLength){
	int targetTetherOut = 0;
	targetTetherOut = tetherLetOut - commandLength;
	
	stepperMotor.setDirection(CCW);
	stepperMotor.setSpeed(RAPPEL_ANGULAR_SPEED);
	stepperMotor.enableStepping();
	
	while(tetherLetOut > targetTetherOut){
		//Drive stepper motor
		tetherLetOut = stepperMotorEncoder.getDistanceTraveled();
	}
	
	stepperMotor.disableStepping();
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

void MRMain::stepperEncoderISR(){
	stepperMotorEncoder.encoderISR();
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

