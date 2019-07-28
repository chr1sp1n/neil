#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>
#include "stepper.cpp"

void stepper_init(unsigned int speed );
void stepper_step(bool direction);
void stepper_run();
void stepper_setSpeed(byte rpm);
void stepper_setStepsToDoAndDirection(int mm, int deg);

#endif	// STEPPER_H