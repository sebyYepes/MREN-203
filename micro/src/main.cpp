// #include <motor.h>
#include <ir.h>
#include <motors.h>
#include <Arduino.h>
#include <ArduinoJson.h>

// // Motor PWM command variables [0-255]
// short u_L = 0;
// short u_R = 0;

// // Counter to keep track of encoder ticks [integer]
// volatile long encoder_ticks_L = 0;
// volatile long encoder_ticks_R = 0;

// // Variables to store estimated angular rates of wheels [rad/s]
// double omega_L = 0.0;
// double omega_R = 0.0;

// // Variables to store estimated wheel speeds [m/s]
// double v_L = 0.0;
// double v_R = 0.0;

// // Variables to store vehicle speed and turning rate
// double v = 0.0;     // [m/s]
// double omega = 0.0; // [rad/s]

// // Variables to store desired vehicle speed and turning rate
// double v_d = 0.0;     // [m/s]
// double omega_d = 0.0; // [rad/s]

// // Variable to store desired wheel speeds [m/s]
// double v_Ld = 0.0;
// double v_Rd = 0.0;

// // Counters for milliseconds during interval
long t_curr  = 0;
long t_prev  = 0;

// // Variables to store errors for controller
// double e_L = 0.0;
// double e_R = 0.0;
// double e_Lint = 0.0;
// double e_Rint = 0.0;

// const double KP = 150.0; // Proportional gain
// const double KI = 200.0; // Integral gain

const double T = 0.1; //[s]

void setup()
{
    setupMotors();
    Serial.begin(115200);
    Serial.println("Testing motors...");

    //moveForward(150);
    //delay(3000);
    //stop();
}

void loop()
{
    int frontDistance, leftDistance, rightDistance;

    readirData(frontDistance, leftDistance, rightDistance);

    if (threshold_distance > frontDistance || threshold_distance > leftDistance || threshold_distance > rightDistance)
    {
        //stop();
        delay(5000);
    }

    t_curr = millis();
    // Perform control update every T milliseconds
    if (t_curr - t_prev  >= T)
    {
	setSpeed(&t_prev, t_curr);
	t_prev  = t_curr; 
        } 
}
