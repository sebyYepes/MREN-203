/**
 * @file PWM-motor-control.ino
 * @author Joshua Marshall (joshua.marshall@queensu.ca)
 * @brief Arduino program to drive one wheel motor through a motor driver.
 * @version 2.0
 * @date 2022-12-05
 *
 * @copyright Copyright (c) 2021-2022
 *
 */

int EA = 3; // Wheel PWM pin (must be a PWM pin)
int I1 = 2; // Wheel direction digital pin 1
int I2 = 4; // Wheel direction digital pin 2
int EB = 9; 
int I3 = 10;
int I4 = 8;

bool runOnce = true; 

void setup()
{
    // Configure digital pins for output
    pinMode(EA, OUTPUT);
    pinMode(I1, OUTPUT); 
    pinMode(I2, OUTPUT);
    pinMode(EB, OUTPUT);
    pinMode(I3, OUTPUT);
    pinMode(I4, OUTPUT);
}

void loop()
{
    int u; // A variable for the motor PWM command [0-255]
    // CODE FOR ROBOT TO MOVE FORWARD, CAN VARY SPEED USING ANALOGWRITE
    // Play with this code to write open loop commands to a wheel motor
    // for (u = 0; u <= 255; u += 5)
    // {
    //     // forward
    //     digitalWrite(I1, LOW);  
    //     digitalWrite(I2, HIGH); 
    //     digitalWrite(I3, LOW);
    //     digitalWrite(I4, HIGH);

    //     // PWM command to the motor driver
    //     analogWrite(EA, 255);
    //     analogWrite(EB, 255); 

        // // TURN CLOCKWISE 
        //   digitalWrite(I1, HIGH);
        //   digitalWrite(I2, LOW);
        //   digitalWrite(I3, LOW);
        //   digitalWrite(I4, HIGH);

        //   analogWrite(EA, 255);
        //   analogWrite(EB, 255); 

        //   delay(3000);

        //   // REVERSE
        //   digitalWrite(I1, HIGH);
        //   digitalWrite(I2, LOW);
        //   digitalWrite(I3, HIGH);
        //   digitalWrite(I4, LOW);

        //   analogWrite(EA, 255);
        //   analogWrite(EB, 255); 

        //   delay(2000);

        //   //TURN COUNTER CLOCKWISE
        //   digitalWrite(I1, LOW);
        //   digitalWrite(I2, HIGH);
        //   digitalWrite(I3, HIGH);
        //   digitalWrite(I4, LOW);

        //   analogWrite(EA, 255);
        //   analogWrite(EB, 255); 

        //   delay(2000);

        //   analogWrite(EA, 0);
        //   analogWrite(EB, 0);

        //   delay(2000);
  
    // }
    // Select a direction 
// code used for figure 8

  // go straight
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  analogWrite(EA, 255);
  analogWrite(EB, 255);

  delay(3000);

  // stop
  analogWrite(EA, 0);
  analogWrite(EB, 0);
  delay(1000);

  // circle CLOCKWISE 
  digitalWrite(I1, HIGH);
  digitalWrite(I2, LOW);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  analogWrite(EA, 100);
  analogWrite(EB, 255); 

  delay(3000);

  // straight
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);
  digitalWrite(I3, LOW);
  digitalWrite(I4, HIGH);

  analogWrite(EA, 255);
  analogWrite(EB, 255);

  delay(3000);

  //TURN COUNTER CLOCKWISE
  digitalWrite(I1, LOW);
  digitalWrite(I2, HIGH);

  analogWrite(EA, 255);
  analogWrite(EB, 0); 

  delay(2000);

  //stop
  analogWrite(EA, 0);
  analogWrite(EB, 0);

  delay(2000);

}