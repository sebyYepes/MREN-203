#ifndef SPEEDCONTROLLER_H
#define SPEEDCONTROLLER_H

#include <Arduino.h>

void setupMotors();
void driveVehicle(short u_L, short u_R);
void decodeEncoderTicks_L();
void decodeEncoderTicks_R();
double compute_wheel_rate(long encoder_ticks, double delta_t);
double compute_wheel_speed(double omega_wheel);
double compute_vehicle_speed(double v_L, double v_R);
double compute_vehicle_rate(double v_L, double v_R);
double compute_L_wheel_speed(double v, double omega);
double compute_R_wheel_speed(double v, double omega);
short PI_controller(double e_now, double e_int, double k_P, double k_I);

void setSpeed(int v_d, int omega_d, long* t_last_ptr, long t_now);
#endif 
