#include "mrconstants.h"
/*
 * MotherRover.ino
 *
 * Created: 1/5/2015 10:56:04 AM
 * Author: John Russo
 *
 */ 

#include "MRMain.h"

void setup()
{
	mrMain.setup();
	attachInterrupt(2,stepperEncoderISR,FALLING);
}

void loop()
{
	mrMain.loop();
}

void stepperEncoderISR()
{
	mrMain.stepperEncoderISR();
}