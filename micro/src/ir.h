#ifndef IR_H
#define IR_H

#include <Arduino.h>
extern int threshold_distance;
void setupSerial();
void readirData(int &distance, int &distance_L, int &distance_R);

#endif 
