#include <ir.h>
#include <math.h>

// Arduino analog input pin to which the Sharp sensor is connected
const byte SHARP_PIN = A0;
const byte SHARP_PIN_left = A2;
const byte SHARP_PIN_right = A1;


// Variables to store the proximity measurement
int sharp_val_F = 0; // integer read from analog pin
int sharp_val_L = 0;
int sharp_val_R = 0;

float sharp_range; // range measurement [cm]
int threshold_distance = 15

void setupSerial()
{
    // Open the serial port at 115200 bps
    Serial.begin(115200);
}

void readirData(int &distance, int &distance_L, int &distance_R)
{
    // Read the sensor output (0-1023, which is 10 bits and fits inside an Arduino int-type)
    int sum_F = 0;
    int sum_L = 0;
    int sum_R = 0;

    for (int i = 0; i < 20; i++) {
      sharp_val_F = analogRead(SHARP_PIN);
      sharp_val_L = analogRead(SHARP_PIN_left);
      sharp_val_R = analogRead(SHARP_PIN_right);
      sum_F += sharp_val_F;
      sum_L += sharp_val_L;
      sum_R += sharp_val_R;
    }
    sum_F /= 20; 
    sum_L /= 20; 
    sum_R /= 20; 
    int distance = 149.72*exp(-0.005*sum_F);
    int distance_L = 149.72*exp(-0.005*sum_L);
    int distance_R = 149.72*exp(-0.005*sum_R);


    // serial prints, prolly dont need
    // Serial.print("the predicted distance from front: ");
    // Serial.println(distance);
    // Serial.print("the predicted distance from left: ");
    // Serial.println(distance_L);
    // Serial.print("the predicted distance from right: ");
    // Serial.println(distance_R);

    // Print all values, prolly dont need
    // Serial.println(sum_F);

    // Delay for a bit before reading the sensor again
    delay(500);
}
