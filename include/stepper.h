#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>
#include "stepper.cpp"

void stepper_setSpeed(int rpm);
void stepper_step(long direction_left, long direction_right);
void stepper_set(int mm_left, int mm_right, int speed);
void stepper_run();
void stepper_init(int speed);

#endif	// STEPPER_H