#include <Arduino.h>
#include "communications.h"

String communications_input = "";         // a String to hold incoming data

long get_arg(String inputString, int index){
	
	int start = 0;
	int t_start = -1;
	int stop = 0;
	String result = "";

	for(int i = 0; i < index + 1; ++i ){
		t_start = inputString.indexOf( ' ', start );
		if(t_start > -1) start = t_start + 1;

		// Serial.print("index: ");
		// Serial.print(i);
		// Serial.print(" - t_start: ");
		// Serial.print(t_start);
		// Serial.print(" - start: ");
		// Serial.print(start);

		if( i == index && t_start > -1 ){
			
			stop = inputString.indexOf(' ', start);


			// Serial.print(" - stop: ");
			// Serial.print(stop);

			if(stop > -1){
				result = inputString.substring(start, stop);
			}else{
				result = inputString.substring(start);
			}

			// Serial.print(" - resut: ");
			// Serial.println(result);

			return result.toInt();
		}else{
			//Serial.println();
		}
	}

	return 0;
}

void communication_send(char cmd, int arg_1, int arg_2, int arg_3){
	Serial.print(cmd);
	Serial.print(" ");
	Serial.print(arg_1);
	Serial.print(" ");
	Serial.print(arg_2);
	Serial.print(" ");
	Serial.println(arg_3);
}

void communications_run(void (*callback)(char cmd, int mm_left, int mm_right, int speed)){

	bool complete = false;
	while ( Serial.available() ) {
		char inChar = (char) Serial.read();
		communications_input += inChar;
		if (inChar == '\n') {		
			complete = true;
			Serial.flush();
		}else{
			Serial.write(inChar);
		}
		Serial.flush();
	}

	if(complete){
		char cmd = 0;
		int arg_1 = 0;
		int arg_2 = 0;
		int arg_3 = 0;


		if( communications_input.length() < 3 ){
			Serial.println("e 1");
		}else{
			cmd = communications_input.charAt(0);
			communications_input.remove(0,1);
			
			arg_1 = get_arg(communications_input, 0);
			//Serial.println();
			arg_2 = get_arg(communications_input, 1);
			//Serial.println();
			arg_3 = get_arg(communications_input, 2);

			switch(cmd){
				case 'M':
					cmd = 'm';
					(*callback)(cmd, arg_1, arg_2, arg_3);
					break;
				case 'T':
					cmd = 't';
					break;
				default:
					cmd = 'e';
					arg_1 = 2;
					break;
			}

			Serial.println();
			communication_send(cmd, arg_1, arg_2, arg_3);
		}

		communications_input = "";
		return;
	}

}

