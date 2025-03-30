#include <Arduino.h>

char recvchars[32];
bool newdata = false;

// this is so jank oh my god

//Should print odom data in the format: <O,velocity,turningRate>
// where velocity is the speed of the robot in m/s
// and turningRate is the angular velocity in rad/s
void sendData(long velocity, long turningRate)
{


    Serial.print("<");
    Serial.print("O,"); // O for odometry
    Serial.print(velocity);
    Serial.print(",");
    Serial.print(turningRate);
    Serial.print(",");
    Serial.print(">");
}

//Recieves data from the serial port. Should be for sending commands to the robot.
// The format should be:
// <C,velocity_desired,turningRate_desired>
int* recieveData()
{
    char recvchars[32];
    bool recvinprogress = false;
    int ndx = 0;
    char startmarker = '<';
    char comma = ',';
    char endmarker = '>';
    char c;
    char wrongdata = 'O';

    while (Serial.available() > 0 && newdata == false)
    {
        c = Serial.read();

        if (c == startmarker)
            recvinprogress = true;
        else if (c == wrongdata) {
            recvinprogress = false;
            return nullptr;
        }
        else if (c == endmarker)
        {
            recvinprogress = false;
            ndx = 0;
            newdata = true;
        }
        if (recvinprogress == true && c != comma)
        {
            recvchars[ndx] = c;
            ndx++;
        }
    }
    //write a function to parse the data and convert the numbers in the string to integers
    

    return recvchars;
}