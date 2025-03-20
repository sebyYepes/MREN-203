#include <urmom.h>
#include <ir.h>

void setup(){
    setupMotors();
}

void loop(){

    int frontDistance, leftDistance, rightDistance;

    readirData(frontDistance, leftDistance, rightDistance);
    if(threshold_distance > frontDistance || threshold_distance > leftD>        
        stop();
        delay(5000);
    }
    else{
        moveForward(200);
    }
    delay(500);

}
