#include <Arduino.h>
#include <ArduinoJson.h>
#include <motors.h>

// Left wheel encoder digital pins
const byte SIGNAL_AL = 11; // green wire
const byte SIGNAL_BL = 12; // yellow wire

// Right wheel encoder digital pins
const byte SIGNAL_AR = 6;  // green wire
const byte SIGNAL_BR = 5; // yellow wire

// Wheel PWM pin (must be a PWM pin)
int EA = 3;
int EB = 9;
// Wheel direction digital pins
int I1 = 2;
int I2 = 4;
int I3 = 8;
int I4 = 10;

// Encoder ticks per (motor) revolution (TPR)
const int TPR = 3000;

// Wheel radius [m]
const double RHO = 0.0625;

// Vehicle track [m]
const double ELL = 0.2775;

// Sampling interval for measurements in milliseconds
const int T = 100;

// Controller gains (use the same values for both wheels)
const double KP = 150.0; // Proportional gain
const double KI = 0.0; // Integral gain

/* VARIABLE DECLARATIONS */

// Motor PWM command variables [0-255]
short u_L = 0;
short u_R = 0;


// Counter to keep track of encoder ticks [integer]
volatile long encoder_ticks_L = 0;
volatile long encoder_ticks_R = 0;

// Variables to store estimated angular rates of wheels [rad/s]
double omega_L = 0.0;
double omega_R = 0.0;

// Variables to store estimated wheel speeds [m/s]
double v_L = 0.0;
double v_R = 0.0;

// Variables to store vehicle speed and turning rate
double v = 0.0;     // [m/s]
double omega = 0.0; // [rad/s]

// Variables to store desired vehicle speed and turning rate
double v_d = 0.0;     // [m/s]
double omega_d = 0.0; // [rad/s]

// Variable to store desired wheel speeds [m/s]
double v_Ld = 0.0;
double v_Rd = 0.0;

// Counters for milliseconds during interval
long t_now = 0;
long t_last = 0;

// Variables to store errors for controller
double e_L = 0.0;
double e_R = 0.0;
double e_Lint = 0.0;
double e_Rint = 0.0;



// Motor PWM command variable [0-255]
byte u = 0;
 
void setupMotors() {
  // put your setup code here, to run once:
  
    // Configure digital pins for output
    pinMode(EA, OUTPUT);
    pinMode(I1, OUTPUT); 
    pinMode(I2, OUTPUT);
    pinMode(EB, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);

}

void driveVehicle(short u_L, short u_R)
{
    // LEFT WHEEL
    if (u_L < 0) // If the controller calculated a negative input...
    {
        digitalWrite(I3, HIGH); // Drive backward (left wheels)
        digitalWrite(I4, LOW);  // Drive backward (left wheels)

        analogWrite(EB, -u_L); // Write left motors command
    }
    else // the controller calculated a positive input
    {
        digitalWrite(I3, LOW);  // Drive forward (left wheels)
        digitalWrite(I4, HIGH); // Drive forward (left wheels)

        analogWrite(EB, u_L); // Write left motors command
    }

    // RIGHT WHEEL
    if (u_R < 0) // If the controller calculated a negative input...
    {
        digitalWrite(I1, LOW);  // Drive backward (right wheels)
        digitalWrite(I2, HIGH); // Drive backward (right wheels)

        analogWrite(EA, -u_R); // Write right motors command
    }
    else // the controller calculated a positive input
    {
        digitalWrite(I1, HIGH); // Drive forward (right wheels)
        digitalWrite(I2, LOW);  // Drive forward (right wheels)

        analogWrite(EA, u_R); // Write right motors command
    }
}

void decodeEncoderTicks_L()
{
    if (digitalRead(SIGNAL_BL) == LOW)
    {
        // SIGNAL_A leads SIGNAL_B, so count one way
        encoder_ticks_L--;
    }
    else
    {
        // SIGNAL_B leads SIGNAL_A, so count the other way
        encoder_ticks_L++;
    }
}

// This function is called when SIGNAL_AR (right encoder) goes HIGH
void decodeEncoderTicks_R()
{
    if (digitalRead(SIGNAL_BR) == LOW)
    {
        // SIGNAL_A leads SIGNAL_B, so count one way
        encoder_ticks_R++;
    }
    else
    {
        // SIGNAL_B leads SIGNAL_A, so count the other way
        encoder_ticks_R--;
    }
}

// Compute the wheel rate from elapsed time and encoder ticks [rad/s]
double compute_wheel_rate(long encoder_ticks, double delta_t)
{
    double omega;
    omega = 2.0 * PI * ((double)encoder_ticks / (double)TPR) * 1000.0 / delta_t;
    return omega;
}

// Compute wheel speed [m/s]
double compute_wheel_speed(double omega_wheel)
{
    double v_wheel;
    v_wheel = omega_wheel * RHO;
    return v_wheel;
}

// Compute vehicle speed [m/s]
double compute_vehicle_speed(double v_L, double v_R)
{
    double v;
    v = 0.5 * (v_L + v_R);
    return v;
}

// Compute vehicle turning rate [rad/s]
double compute_vehicle_rate(double v_L, double v_R)
{
    double omega;
    omega = 1.0 / ELL * (v_R - v_L);
    return omega;
}

// Compute v_L from v and omega
double compute_L_wheel_speed(double v, double omega)
{
    double v_wheel = 0.0;
    v_wheel = v - (ELL * omega) / 2.0 ;
    return v_wheel;
}

// Compute v_R from v and omega
double compute_R_wheel_speed(double v, double omega)
{
    double v_wheel = 0.0;
    v_wheel = v + (ELL * omega) / 2.0 ;
    return v_wheel;
}

// Wheel speed PI controller function
short PI_controller(double e_now, double e_int, double k_P, double k_I)
{
    short u;
    u = (short)(k_P * e_now + k_I * e_int);

    // Saturation (i.e., maximum input) detection
    if (u > 255)
    {
        u = 255;
    }
    else if (u < -255)
    {
        u = -255;
    }
    return u;
}

void setSpeed(int vd, int omegad, long* t_last_ptr, long t_now) {
    StaticJsonDocument<1024> setSpeed;
    /* do
    {
        String desiredVelocity = Serial.readStringUntil('\n');
        desiredVelocity.trim();


        DeserializationError error = deserializeJson(setSpeed, desiredVelocity);
        if (error)
        {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.c_str());
            return;
        }
    }
    // type == 0 for sending pi to arduino
    while (setSpeed["type"].as<float>() != 0);
    
    Serial.println(setSpeed["trans_speed"].as<float>());
    Serial.println(setSpeed["angular_speed"].as<float>());
    Serial.println(setSpeed["type"].as<float>());
*/
int t_last = *t_last_ptr;
    // Set the desired vehicle speed and turning rate
    v_d = vd;     // [m/s]
    omega_d = omegad; // [rad/s]

    // Estimate the rotational speed of each wheel [rad/s]
    omega_L = compute_wheel_rate(encoder_ticks_L, (double)(t_now - t_last));
    omega_R = compute_wheel_rate(encoder_ticks_R, (double)(t_now - t_last));

    // Compute the speed of each wheel [m/s]
    v_L = compute_wheel_speed(omega_L);
    v_R = compute_wheel_speed(omega_R);

    // Compute the speed of the vehicle [m/s]
    v = compute_vehicle_speed(v_L, v_R);

    // Compute the turning rate of the vehicle [rad/s]
    omega = compute_vehicle_rate(v_L, v_R);

    // Record the current time [ms]
    *t_last_ptr = t_now;

    // Reset the encoder ticks counter
    encoder_ticks_L = 0;
    encoder_ticks_R = 0;

    // Compute the desired wheel speeds from v_d and omega_d
    v_Ld = compute_L_wheel_speed(v_d, omega_d);
    v_Rd = compute_R_wheel_speed(v_d, omega_d);

    // Compute errors
    e_L = v_Ld - v_L;
    e_R = v_Rd - v_R;

    // Integrate errors with anti-windup
    if (abs(u_L) < 255)
    {
        e_Lint += e_L;
    }
    if (abs(u_R) < 255)
    {
        e_Rint += e_R;
    }

    // Compute control signals using PI controller
    u_L = PI_controller(e_L, e_Lint, KP, KI);
    u_R = PI_controller(e_R, e_Rint, KP, KI);

    // Drive the vehicle
    driveVehicle(u_L, u_R);

    // Print some stuff to the serial monitor (or plotter)
    Serial.print("Vehicle_speed_[m/s]:");
    Serial.print(v);
    Serial.print(",");
    Serial.print("Turning_rate_[rad/s]:");
    Serial.print(omega);
    Serial.print(",");
    Serial.print("u_L:");
    Serial.print(u_L);
    Serial.print(",");
    Serial.print("u_R:");
    Serial.print(u_R);
    Serial.print("\n");
}
