#include <Arduino.h>
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

void setup() {
  // put your setup code here, to run once:
  
    // Configure digital pins for output
    pinMode(EA, OUTPUT);
    pinMode(I1, OUTPUT); 
    pinMode(I2, OUTPUT);
    pinMode(EB, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  // go straight
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  analogWrite(EA, 255);
  analogWrite(EB, 255);

  delay(3000);
}
