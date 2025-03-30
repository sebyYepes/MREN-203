#include <Arduino.h>
#include <stdlib.h> //for atoi

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
    int vals[] = parseData(recvchars, ndx);

    return vals;
}

int* parseData (char* data, int ndx) {
    char startmarker = '<';
    char comma = ',';
    char endmarker = '>';
    char c;
    char wrongdata = 'O';
    
    int vals[] = {0, 0}; 
    int firstcomma = 0;
    char* str1 = "000";
 
    for (int i = 0; i < ndx; i++)
    {   
         //check if the first comma has been found, if it has been then ignore it
       if (recvchars[i] == comma && firstcomma == 0) {
               firstcomma = 1;
          }
          else if (recvchars[i] == comma && firstcomma == 1)
        { //if we are here, we are past the identifier character
            if (recvchars[i] != comma) {
                 
            }
   
            vals[0] = atoi(recvchars);
            vals[1] = atoi(recvchars + i + 1);
            break;
        }
     }
}