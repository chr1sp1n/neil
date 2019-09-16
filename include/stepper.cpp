#include <Arduino.h>
#include "configurations.h"
#include "stepper.h"


unsigned long tt = 0;
unsigned long t_step = 0;
long stepsToDo_left = 0;
long stepsToDo_right = 0;

bool stepper_running = false;
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

int mmSToRpm(int mmS){
	return (mmS * 60) / (DIAMETER * PI); 
}

long stepsPerMm(int mm){
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

void stepper_setSpeed(int _mmS){
	mmS = _mmS;
	int rpm = mmSToRpm(mmS);
	t_step = (60000000 / rpm) / STEPS_PER_ROTATION;
}

int set_fase(int fase, int direction){
	if(direction > 0) --fase;
	if(direction < 0) ++fase;
	if(fase < 0) fase = 7;
	if(fase > 7) fase = 0;
	return fase;
}

int fase_left = 0;
int fase_right = 0;
void stepper_step(long direction_left, long direction_right){

	fase_left = set_fase(fase_left, direction_left);
	fase_right = set_fase(fase_right, direction_right);

	if(direction_left != 0){
		digitalWrite(MOTORS[MOTOR_LEFT][0], STEPPER_FASES[fase_left][0]);
		digitalWrite(MOTORS[MOTOR_LEFT][1], STEPPER_FASES[fase_left][1]);
		digitalWrite(MOTORS[MOTOR_LEFT][2], STEPPER_FASES[fase_left][2]);
		digitalWrite(MOTORS[MOTOR_LEFT][3], STEPPER_FASES[fase_left][3]);
	}

	if(direction_right != 0){
		digitalWrite(MOTORS[MOTOR_RIGHT][0], STEPPER_FASES[fase_right][0]);
		digitalWrite(MOTORS[MOTOR_RIGHT][1], STEPPER_FASES[fase_right][1]);
		digitalWrite(MOTORS[MOTOR_RIGHT][2], STEPPER_FASES[fase_right][2]);
		digitalWrite(MOTORS[MOTOR_RIGHT][3], STEPPER_FASES[fase_right][3]);
	}

	digitalWrite( LED_BUILTIN, !digitalRead(LED_BUILTIN) );
	stepper_running = true;
}

void stepper_init(int mmS){
	pinMode(MOTORS[X_AXIS][0], OUTPUT);
	pinMode(MOTORS[X_AXIS][1], OUTPUT);
	pinMode(MOTORS[X_AXIS][2], OUTPUT);
	pinMode(MOTORS[X_AXIS][3], OUTPUT);

	pinMode(MOTORS[Y_AXIS][0], OUTPUT);
	pinMode(MOTORS[Y_AXIS][1], OUTPUT);
	pinMode(MOTORS[Y_AXIS][2], OUTPUT);
	pinMode(MOTORS[Y_AXIS][3], OUTPUT);

	stepper_off();

	stepper_setSpeed( mmS );
}

void stepper_run(){
	if(stepsToDo_left == 0 && stepsToDo_right == 0){
		//if(stepper_running){
			stepper_off();
			stepper_running = false;
			digitalWrite(LED_BUILTIN, LOW);
		//}
	}else{
		//if(stepper_running) {
			if( micros() > tt + t_step ){
				stepper_step(stepsToDo_left, stepsToDo_right);
				tt = micros();
				if(stepsToDo_left > 0) --stepsToDo_left;
				if(stepsToDo_left < 0) ++stepsToDo_left;
				if(stepsToDo_right > 0) --stepsToDo_right;
				if(stepsToDo_right < 0) ++stepsToDo_right;
			}
		// }else{
		// 	digitalWrite(LED_BUILTIN, HIGH);
		// 	stepper_running = true;
		// 	tt = micros();
		// 	return;
		// }		
	}
}

void stepper_set(int mm_left, int mm_right, int speed){
	stepsToDo_left = stepsPerMm(mm_left);
	stepsToDo_right = stepsPerMm(mm_right);
	if(speed > 0) stepper_setSpeed(speed);
}

