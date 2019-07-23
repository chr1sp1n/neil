#include "configurations.h"
#include "radar.h"
#include "stepper.h"

void setup() {
	Serial.begin(SERIAL_BAUD_RATE);
	pinMode(LED_BUILTIN, OUTPUT);

	scan();
	for(int i = 0; i < 3; ++i){
		Serial.println( OBSTACLE[i] );
	}
	stepper_init();
	stepper_setSpeed( mmSToRpm(15) );
	stepsToDo = stepsPerMm(10);
}


void loop() {
	stepper_run();
	if(!running){
		radar(CENTER);
		if( stepsToDo > 0 ){
			Serial.print("CENTER: ");
			Serial.println(OBSTACLE[CENTER]);
		}
		if( OBSTACLE[CENTER] < 200 ){
			stepsToDo = 0;
			scan();
			delay(100);
			Serial.print("LEFT:   ");
			Serial.print(OBSTACLE[LEFT]);
			Serial.print("  -  CENTER: ");
			Serial.print(OBSTACLE[CENTER]);
			Serial.print("  -  RIGHT:  ");
			Serial.println(OBSTACLE[RIGHT]);
		}else{
			delay(50);
			stepsToDo = stepsPerMm(10);
		}
	}
}