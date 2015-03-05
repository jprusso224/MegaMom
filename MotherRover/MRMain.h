/**
 * MRMain.h
 *
 * Created: 1/5/2015 
 * Author: John Russo
 * Last Updated: 1/8/2015 by John Russo
 */ 

#ifndef _MRMAIN_h
#define _MRMAIN_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "StepperMotor.h"
#include "Encoder.h"

class MRMain
{
 private:
	 String gsInputString;
	 boolean gsInputStringComplete;
	 String crInputString;
	 boolean crInputStringComplete;
	 Encoder stepperMotorEncoder;
	 
	 
	 char inChar;
	 
	 int currentDepth;
	 int tetherLetOut;
	 
	 /**
	  * Battery                                                    
	  */
	 int readBatteryVoltage();

 public:
    /** 
     * Initialize function for the MR Arduino
     */
	void setup();
	
	/**
	 * Main loop function for Mr Arduino
	 */
	void loop();
	
	/**
	 * Parses an incoming command to determine which action to take                                
	 */
	void parseCommand();
	
	/**
	 * Relays command to CR and waits for acknowledgment                
	 */
	void processImageCommand();
	
	/**
	 * Unspool tether and then relay command to CR                                      
	 */
	void processDriveCommand();
	
	/**
	 * Unspool/spool tether                                                   
	 */
	void processRappelCommand();
	
	/**
	 * Relays command to CR and waits for acknowledgment. Then attaches MR status and returns to GS                                            
	 */
	void processStatusRequest();
	
    /**
	 * Reels in the CR using the stepper motor                                           
	 */
	void processReturnCommand();
	
	/**
	 * Blink LED   
	 *
	 * @param num number of times to blink.                                                 
	 */
	void blinkLED(int num);
	
	/**
	 * Hardware ISR for encoder                                                  
	 */
	void stepperEncoderISR();
	
	/**
	 * Function to auto spool tether via the winch                                                    
	 */
	void autoSpoolOut(int commandLength);	
	
	/**
	 * Function to auto reel tether via the winch                                                    
	 */
	void autoReelIn(int commandLength);
	
};

extern MRMain mrMain;

#endif

