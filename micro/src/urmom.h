#ifndef URMOM_H
#define URMOM_H

#include <Arduino.h>

void setupMotors();
void moveForward(int speed);
void reverse(int speed);
void stop();

#endif
