
#ifndef configuration_H
#define configuration_H

#include <Arduino.h>

// Stepper
#define		X_AXIS				0
#define		X1					2
#define		X2					3
#define		X3					4
#define		X4					5

#define		Y_AXIS				1
#define		Y1					6
#define		Y2					7
#define		Y3					8
#define		Y4					9

#define 	STEPS_PER_ROTATION	4096	//  Steps per rotation
#define		SPEED				10		//	RPM
#define		MIN_SPEED			1		//	RPM
#define		MAX_SPEED			16		//	RPM

#define 	FORWARDS 			0
#define 	BACKWARDS 			180

// Weels
#define 	DIAMETER			56		// 	mm
#define		PERIMETER			176		//  mm	56 * 3.14

// Steppers-Weels
#define		STEPS_PER_1_MM		24		// Steps to make 1 mm


// Radar
#define		RADAR_ANGLE			A5
#define		RADAR_TRIGGER		A4
#define		RADAR_ECHO			A3
#define		RADAR_MIDDLE		85
#define		RADAR_RANGE			120
#define		RADAR_SOUND_SPEED	0.034		// cm/us


// Accelerometer 
#define		ACC_SCL				13
#define		ACC_SDA				12


#define		SERIAL_BAUD_RATE	115200

#endif