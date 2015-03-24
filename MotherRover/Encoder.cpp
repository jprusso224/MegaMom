// 
// 
// 

#include "Encoder.h"

void Encoder::initEncoder(int interruptNumber, int resolution,int dirPin,int ePin){
	_interruptNumber = interruptNumber;
	_resolution = resolution;
	_dirPin = dirPin;
	_ePin = ePin;
	pulseCount = 0L;
	pinMode(ePin,INPUT);
	digitalWrite(ePin,HIGH);
	pinMode(dirPin,INPUT);
	dirFlag = false;
	
}

void Encoder::encoderISR(){
	
	if(dirFlag){
			pulseCount--;
	}
	else{
			pulseCount++;
	}
	
}

int Encoder::getDistanceTraveled(){
	
	float mrad = (float)(pulseCount*2.0*PI)/(float)_resolution; //(rad/1000)
	//Serial.print((String)mrad)
	mrad = mrad/GEAR_RATIO;
    int distanceTraveled = int(mrad*SPOOL_RADIUS); 
	return distanceTraveled;
}

long Encoder::getPulseCount(){
	return pulseCount;
}

void Encoder::setDirFlag(boolean flag){
	dirFlag = flag;
}