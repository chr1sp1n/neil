#ifndef COMMUNICATIONS_H
#define COMMUNICATIONS_H

#include <Arduino.h>
#include "communications.cpp"

#define CMD_MOVE 		"M"
#define CMD_MOVE_END 	"E"
#define CMD_OBSTACLE 	"O"
#define CMD_DISTANCE 	"D"
#define END_CMD 		"\n"

#define CMD_AVAILABLE	{ 'M', 'O', 'D' }


void communications_run(void (*)( char cmd, unsigned int mm, int deg, int speed ));
void communications_send(char cmd, unsigned int mm, int deg, int speed);


#endif	// COMMUNICATIONS_H
