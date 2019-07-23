#include <Arduino.h>
#include "configurations.h"
#include "stepper.h"

unsigned long tt = 0;
unsigned long t_step = 0;
unsigned long stepsToDo = 0;
bool running = false;

byte motor = X_AXIS;
int MOTORS[2][4] = {
	{ X1, X2, X3, X4 },
	{ Y1, Y2, Y3, Y4 }
};


int STEPPER_FASES[8][4] = {
	//   X1	  X2	 X3	   X4
	{  LOW,  LOW,  LOW , HIGH},
	{  LOW,  LOW, HIGH , HIGH},
	{  LOW,  LOW, HIGH ,  LOW},
	{  LOW, HIGH, HIGH ,  LOW},
	{  LOW, HIGH,  LOW ,  LOW},
	{ HIGH, HIGH,  LOW ,  LOW},
	{ HIGH,  LOW,  LOW ,  LOW},
	{ HIGH,  LOW,  LOW , HIGH}
};

unsigned int mmSToRpm(unsigned int mmS){
	return (mmS * 60) / (DIAMETER * PI); 
}

unsigned int stepsPerMm(unsigned int mm){
	return mm * ( STEPS_PER_ROTATION / (DIAMETER * PI) );
}


void stepper_off(){
	digitalWrite(MOTORS[X_AXIS][0], LOW);
	digitalWrite(MOTORS[X_AXIS][1], LOW);
	digitalWrite(MOTORS[X_AXIS][2], LOW);
	digitalWrite(MOTORS[X_AXIS][3], LOW);	
	digitalWrite(MOTORS[Y_AXIS][0], LOW);
	digitalWrite(MOTORS[Y_AXIS][1], LOW);
	digitalWrite(MOTORS[Y_AXIS][2], LOW);
	digitalWrite(MOTORS[Y_AXIS][3], LOW);	
}

void stepper_init(){
	pinMode(MOTORS[X_AXIS][0], OUTPUT);
	pinMode(MOTORS[X_AXIS][1], OUTPUT);
	pinMode(MOTORS[X_AXIS][2], OUTPUT);
	pinMode(MOTORS[X_AXIS][3], OUTPUT);

	pinMode(MOTORS[Y_AXIS][0], OUTPUT);
	pinMode(MOTORS[Y_AXIS][1], OUTPUT);
	pinMode(MOTORS[Y_AXIS][2], OUTPUT);
	pinMode(MOTORS[Y_AXIS][3], OUTPUT);
	stepper_off();
}

int fase = 0;
void stepper_step(bool direction){
	if(direction) --fase;
	if(!direction) ++fase;
	if(fase < 0) fase = 7;
	if(fase > 7) fase = 0;
	digitalWrite(MOTORS[motor][0], STEPPER_FASES[fase][0]);
	digitalWrite(MOTORS[motor][1], STEPPER_FASES[fase][1]);
	digitalWrite(MOTORS[motor][2], STEPPER_FASES[fase][2]);
	digitalWrite(MOTORS[motor][3], STEPPER_FASES[fase][3]);
	//digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

void stepper_run(){
	if( stepsToDo > 0){
		if(running) {
			if( micros() > tt + t_step ){
				stepper_step(true);
				tt = micros();
				--stepsToDo;
			}
		}else{
			digitalWrite(LED_BUILTIN, HIGH);
			running = true;
			tt = micros();
			return;
		}
	}else{
		if(running){
			stepper_off();
			running = false;
			digitalWrite(LED_BUILTIN, LOW);
		}
	}
}

void stepper_setSpeed(byte rpm){
	t_step = (60000000 / rpm) / STEPS_PER_ROTATION;
}