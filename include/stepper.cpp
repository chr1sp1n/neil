#include <Arduino.h>
#include "configurations.h"
#include "stepper.h"

unsigned long tt = 0;
unsigned long t_step = 0;
unsigned long stepsToDo = 0;
bool stepper_running = false;
bool direction = true;
byte motor = X_AXIS;
int y_position = 0;
int mmS = 0;


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

unsigned int stepsPerMm(int mm){\
	motor = X_AXIS;
	int steps =  mm * ( STEPS_PER_ROTATION / (DIAMETER * PI) );
	if ( steps > 0 ){
		direction = true;
	}else{
		steps = steps * -1;
		direction = false;
	}
	return steps;
}

unsigned int stepsPerDeg(int deg){
	motor = Y_AXIS;
	int steps = (STEPS_PER_ROTATION / 360) * deg;	
	if ( steps > 0 ){
		direction = true;
	}else{
		steps = steps * -1;
		direction = false;
	}
	return steps;
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

void stepper_setSpeed(int _mmS){
	mmS = _mmS;
	byte rpm = mmSToRpm(mmS);
	t_step = (60000000 / rpm) / STEPS_PER_ROTATION;
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
	digitalWrite( LED_BUILTIN, !digitalRead(LED_BUILTIN) );
}

void stepper_init(unsigned int mmS){
	pinMode(MOTORS[X_AXIS][0], OUTPUT);
	pinMode(MOTORS[X_AXIS][1], OUTPUT);
	pinMode(MOTORS[X_AXIS][2], OUTPUT);
	pinMode(MOTORS[X_AXIS][3], OUTPUT);

	pinMode(MOTORS[Y_AXIS][0], OUTPUT);
	pinMode(MOTORS[Y_AXIS][1], OUTPUT);
	pinMode(MOTORS[Y_AXIS][2], OUTPUT);
	pinMode(MOTORS[Y_AXIS][3], OUTPUT);
	stepper_off();

	if( mmS > 0 ) stepper_setSpeed( mmS );
}

void stepper_setStepsToDoAndDirection(int mm, int deg){
	motor = X_AXIS;

	if( mm == 0 && deg == 0 && y_position > 0 ){
		if(y_position > 0 && y_position < 91){
			stepsToDo = stepsPerDeg(y_position * -1);
		}else if(y_position > 269 && y_position < 360){
			stepsToDo = stepsPerDeg((360 - y_position) * -1);
		}
	}else{
		if(mm > 0){
			if( deg == 180 ){
				stepsToDo = stepsPerMm(mm * -1);	
			}else{
				stepsToDo = stepsPerMm(mm);
			}
		}else{
			if(deg > 0 && deg < 91){
				stepsToDo = stepsPerDeg( deg );
			}else if(deg > 269 && deg < 360){
				stepsToDo = stepsPerDeg( (360 - deg)  * - 1 );
			}
		}
		y_position = deg;
	}

}

void stepper_run(){
	if( stepsToDo > 0 ){
		if(stepper_running) {
			if( micros() > tt + t_step ){
				stepper_step(direction);
				tt = micros();
				--stepsToDo;
			}
		}else{
			digitalWrite(LED_BUILTIN, HIGH);
			stepper_running = true;
			tt = micros();
			return;
		}
	}else{
		if(stepper_running){
			stepper_off();
			stepper_running = false;
			digitalWrite(LED_BUILTIN, LOW);
		}
	}
}

