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
	
}

void Encoder::encoderISR(){
	
	if(digitalRead(_dirPin) == CCW){
			pulseCount--;
	}
	else{
			pulseCount++;
	}
	
}

int Encoder::getDistanceTraveled(){
	
	long mrad = long(pulseCount*10*2*PI)/_resolution; //(rad/1000)
	//Serial.print((String)mrad)
    int distanceTraveled = int(mrad*SPOOL_RADIUS/10); 
	return distanceTraveled;
}

int Encoder::getPulseCount(){
	return pulseCount;
}