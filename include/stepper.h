#ifndef STEPPER_H
#define STEPPER_H

#include <Arduino.h>
#include "stepper.cpp"

void stepper_step(bool direction);
void stepper_run();
void stepper_setSpeed(byte rpm);

#endif	// STEPPER_H