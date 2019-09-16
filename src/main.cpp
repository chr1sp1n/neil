
#include "configurations.h"
#include "communications.h"
#include "radar.h"
#include "stepper.h"
#include "accelerometer.h"

void setup() {
	Wire.begin();
	Wire.beginTransmission(ACC_ADDRESS);
	Wire.write(0x6B);  // PWR_MGMT_1 register
	Wire.write(0);     // set to zero (wakes up the MPU-6050)
	Wire.endTransmission(true);


	Serial.begin(SERIAL_BAUD_RATE);
	pinMode(RADAR_TRIGGER, OUTPUT);
	pinMode(RADAR_ECHO, INPUT);
	
	stepper_init( 17 ); // init stepper, set speed to 17mm/s
	radar(CENTER);
}


void fetch_command(char cmd, int arg_1, int arg_2, int arg_3){

	switch(cmd){
		case 'm':	// move
			if( arg_3 > 0 ) stepper_setSpeed(arg_3);
			stepper_set(arg_1, arg_2, arg_3);
			break;
		case 's':
			stepper_set(0, 0, 0);
			break;
		case 'd':	// obstacle
			arg_2 = radar(arg_1);
			break;
		case 'a':	// acc
			wire_get();
			arg_1 = AcX;
			arg_2 = AcY;
			arg_3 = AcZ;
			break;
		case 'g':	// gyr
			wire_get();
			arg_1 = GyX;
			arg_2 = GyY;
			arg_3 = GyZ;					
			break;
		case 't':	// temp
			wire_get();
			arg_1 = Tmp / TEMPERATURE_K;
			break;
	};

	communication_send(cmd, arg_1, arg_2, arg_3);	
}


void loop() {
	communications_run(fetch_command);
	stepper_run();
}