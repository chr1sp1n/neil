#include <Arduino.h>
#include <Servo.h>
#include <NewPing.h>
#include "configurations.h"
#include "radar.h"

Servo radarAngle;
NewPing sonar( RADAR_TRIGGER, RADAR_ECHO, RADAR_MAX_DIST );



int OBSTACLE[3];

int radar(int position){

	// if(position > -2){
		radarAngle.attach(RADAR_ANGLE);
		switch(position){
			case LEFT:
				radarAngle.write(RADAR_MIDDLE - (RADAR_RANGE / 2));
				break;
			case CENTER:
				radarAngle.write(RADAR_MIDDLE);
				break;
			case RIGHT:
				radarAngle.write((RADAR_RANGE / 2) + RADAR_MIDDLE);
				break;		
		}
		// delay(300);
	// }else{
		int dist = sonar.ping_cm() * 10;		
		OBSTACLE[position] = dist;
		return dist;
	// }
	// return 0;
}

void scan(){
	for(int i = 0; i < 4; ++i){
		if( i == 0 ) {
			OBSTACLE[LEFT] = radar(LEFT);
		}else if( i == 1 ){
			OBSTACLE[CENTER] = radar(CENTER);
		}else if( i == 2 ) {
			OBSTACLE[RIGHT] = radar(RIGHT);
		}else if( i == 3 ) {
			radarAngle.write(RADAR_MIDDLE);
		}
		//delay(333);
	}
}