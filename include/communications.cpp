#include <Arduino.h>
#include "communications.h"

String communications_input = "";         // a String to hold incoming data

long get_arg(String inputString, int index){
	
	int start = 0;
	int stop = 0;
	String result;

	for(int i = 0; i < index + 1; ++i ){		

		start = inputString.indexOf(' ', start) + 1;

		if( i == index && start > -1 ){
			stop = inputString.indexOf(' ', start) ;
			if(stop > -1){
				result = inputString.substring(start, stop );
			}else{
				result = inputString.substring(start);
			}
			return result.toInt();
		}

	}

	return 0;
}

void communication_send(char cmd, unsigned int mm, int deg, int speed){
	Serial.print(cmd);
	Serial.print(" ");
	Serial.print(mm);
	Serial.print(" ");
	Serial.print(deg);
	Serial.print(" ");
	Serial.println(speed);
}

void communications_run(void (*callback)(char cmd, unsigned int mm, int deg, int speed)){

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
		unsigned int mm = 0;
		int deg = 0;
		int speed = 0;

		if( communications_input.length() < 3 ){
			Serial.println("E 1");
		}else{
			if( 
				communications_input.startsWith( "M" ) || communications_input.startsWith( "m" ) ||
				communications_input.startsWith( "O" ) || communications_input.startsWith( "o" ) ||
				communications_input.startsWith( "D" ) || communications_input.startsWith( "d" )
			){
				cmd = communications_input.charAt(0);
				mm = get_arg(communications_input, 0);
				deg = get_arg(communications_input, 1);
				speed = get_arg(communications_input, 2);
				Serial.println();
				communication_send(cmd, mm, deg, speed);
				(*callback)(cmd, mm, deg, speed);
			}else{
				Serial.println();
				Serial.println("E 2");
			}
		}

		communications_input = "";
		return;
	}

}

