#include urmom.h

void setup(){
    setupMotors()
}

void loop(){
    readirData(frontDistance, leftDistance, rightDistance)
    if(threshold_distance < frontDistance){
        
    }
    moveForward(200)
    
}
