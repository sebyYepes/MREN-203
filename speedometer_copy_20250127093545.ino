 /**
 * @file motor-angular-rate.ino
 * @author Joshua Marshall (joshua.marshall@queensu.ca)
 * @brief Arduino program to estimate motor speed from encoder.
 * @version 2.1
 * @date 2022-12-09
 *
 * @copyright Copyright (c) 2021-2022
 *
 */

// Wheel PWM pin (must be a PWM pin)
int EA = 3;
int EB = 9;
// Wheel direction digital pins
int I1 = 2;
int I2 = 4;
int I3 = 8;
int I4 = 9;
// Motor PWM command variable [0-255]
byte u = 0;

// Left wheel encoder digital pins
const byte SIGNAL_A = 11;
const byte SIGNAL_B = 12;

// for right wheel
const byte SIGNAL_C= 5;
const byte SIGNAL_D= 6;

// Encoder ticks per (motor) revolution (TPR)
const int TPR = 3000;

// Wheel radius [m]
const double RHO = 0.0625;

// Counter to keep track of encoder ticks [integer]
volatile long encoder_ticks = 0;
volatile long encoder_ticksRight = 0;

// Variable to store estimated angular rate of left wheel [rad/s]
double omega_L = 0.0;

// for right wheel
double omega_R = 0.0;


// Sampling interval for measurements in milliseconds
const int T = 1000;

// Counters for milliseconds during interval
long t_now = 0;
long t_last = 0;

// This function is called when SIGNAL_A goes HIGH
void decodeEncoderTicks()
{
    if (digitalRead(SIGNAL_B) == LOW)
    {
        // SIGNAL_A leads SIGNAL_B, so count one way
        encoder_ticks--;
    }
    else
    {
        // SIGNAL_B leads SIGNAL_A, so count the other way
        encoder_ticks++;
    }
}

void decodeEncoderTicksRight()
{
      if (digitalRead(SIGNAL_D) == LOW)
    {
        // SIGNAL_C leads SIGNAL_D, so count one way
        encoder_ticksRight--;
    }
    else
    {
        // SIGNAL_D leads SIGNAL_C, so count the other way
        encoder_ticksRight++;
    }
}

void setup()
{
    // Open the serial port at 9600 bps
    Serial.begin(9600);

    // Set the pin modes for the motor driver
    pinMode(EA, OUTPUT);
    pinMode(I1, OUTPUT);
    pinMode(I2, OUTPUT);
    pinMode(EB, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);

    // Set the pin modes for the encoders
    pinMode(SIGNAL_A, INPUT);
    pinMode(SIGNAL_B, INPUT);
    pinMode(SIGNAL_C, INPUT);
    pinMode(SIGNAL_D, INPUT);

    // Every time the pin goes high, this is a pulse
    attachInterrupt(digitalPinToInterrupt(SIGNAL_A), decodeEncoderTicks, RISING);
    attachInterrupt(digitalPinToInterrupt(SIGNAL_C), decodeEncoderTicksRight, RISING);

    // Print a message
    Serial.print("Program initialized.");
    Serial.print("\n");
}

void loop()
{
    // Get the elapsed time [ms]
    t_now = millis();

    if (t_now - t_last >= T)
    {
        // Estimate the rotational speed [rad/s]
        omega_L = 2.0 * PI * ((double)encoder_ticks / (double)TPR) * 1000.0 / (double)(t_now - t_last);
        omega_R = 2.0 * PI * ((double)encoder_ticksRight / (double)TPR) * 1000.0 / (double)(t_now - t_last);

        // translational speeds of wheels
        double v_R = 0.0625*omega_R;
        double v_L =  0.0625*omega_L;

        // vehicle translational speed
        double v_TRANS = 0.5*(v_R + v_L);

        // vehicle turning rate
        double turning_rate = (1/0.2775)*(v_R - v_L);

        // Print some stuff to the serial monitor
        Serial.print("Encoder ticks Left: ");
        Serial.print(encoder_ticks);
        Serial.print("\t");
        Serial.print("Estimated left wheel speed: ");
        Serial.print(omega_L);
        Serial.print(" rad/s");
        Serial.print("\n");

        Serial.print("Encoder ticks Right: ");
        Serial.print(encoder_ticksRight);
        Serial.print("\t");
        Serial.print("Estimated right wheel speed: ");
        Serial.print(omega_R);
        Serial.print(" rad/s");
        Serial.print("\n");

        Serial.print("vehicle translational Velocity");
        Serial.print(v_TRANS);
        Serial.print("\n");

        Serial.print("vehicle turning rate");
        Serial.print(turning_rate);
        Serial.print("\n");

        // Record the current time [ms]
        t_last = t_now;

        // Reset the encoder ticks counter
        encoder_ticks = 0;
        encoder_ticksRight = 0;
    }

    // Set the wheel motor PWM command [0-255]
    u = 128;

    // Select a direction
    digitalWrite(I1, LOW);
    digitalWrite(I2, HIGH);
    digitalWrite(I3, LOW);
    digitalWrite(I4, HIGH);

    // PWM command to the motor driver
    analogWrite(EA, u);
    analogWrite(EB, u);
}
