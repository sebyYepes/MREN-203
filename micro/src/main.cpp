#include <urmom.h>
#include <ir.h>

void setup(){
    setupMotors();
}

void loop(){
    int frontDistance, leftDistance, rightDistance;

    readirData(frontDistance, leftDistance, rightDistance);

    // ✅ Fix: Ensure the full condition is included
    if (threshold_distance > frontDistance || threshold_distance > leftDistance || threshold_distance > rightDistance) {
        stop();  // ✅ Fix: Use correct function to stop the motors
        delay(5000);
    } else {
        moveForward(200);
    }

    delay(500);
}

