#include <Wire.h>
#include "configurations.h"
#include "communications.h"
#include "radar.h"
#include "stepper.h"

void setup() {
	Serial.begin(SERIAL_BAUD_RATE);
	pinMode(LED_BUILTIN, OUTPUT);
	stepper_init( 17 ); // init stepper, set speed to 17mm/s
}

unsigned int p_steps = 0;
void fetch_command(char cmd, int mm_left, int mm_right, int mmS){

	switch(cmd){
		case 'm':	// move
			//radar(CENTER);
			if( mmS > 0 ) stepper_setSpeed(mmS);
			stepper_set(mm_left, mm_right, mmS);
			//p_steps = stepsToDo;
			break;
		case 'O':	// obstacle
			break;
		case 'D':	// distance
			break;
		default:
			break;
	};

}

bool resp_sent = false;

void loop() {
	
	communications_run(fetch_command);

	stepper_run();

	if(!stepper_running && !resp_sent){
		resp_sent = true;
		communication_send('m', stepsToDo_left, stepsToDo_right, mmS);
	}
	// }else if(stepper_running){
	// 	resp_sent = false;
	// 	if(p_steps - stepsToDo > (STEPS_PER_ROTATION / 16)){
	// 		p_steps = stepsToDo;
	// 		int dist = radar();
	// 		if(dist > 0 && dist < 100){
	// 			unsigned int t_step = stepsToDo;
	// 			stepsToDo = 0;
	// 			//communication_send('m', t_step, y_position, mmS);
	// 			communication_send('o', dist, 0, 0);
	// 		}
	// 	}
	// }



}