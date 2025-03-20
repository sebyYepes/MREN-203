#include urmom.h

void setup(){
    setupMotors()
}

void loop(){
    readirData(frontDistance, leftDistance, rightDistance)
    if(threshold_distance > frontDistance || threshold_distance > leftDistance || threshold_distance > rightDistance){
        stop();
        delay(5000);
    }
    else{
        moveForward(200)
    }
    
}
