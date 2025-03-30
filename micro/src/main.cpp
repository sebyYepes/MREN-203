// #include <motor.h>
#include <ir.h>
#include <motors.h>
#include <Arduino.h>
#include <ArduinoJson.h>

// // Motor PWM command variables [0-255]
// short u_L = 0;
// short u_R = 0;

// // Counter to keep track of encoder ticks [integer]
// volatile long encoder_ticks_L = 0;
// volatile long encoder_ticks_R = 0;

// // Variables to store estimated angular rates of wheels [rad/s]
// double omega_L = 0.0;
// double omega_R = 0.0;

// // Variables to store estimated wheel speeds [m/s]
// double v_L = 0.0;
// double v_R = 0.0;

// // Variables to store vehicle speed and turning rate
// double v = 0.0;     // [m/s]
// double omega = 0.0; // [rad/s]

// // Variables to store desired vehicle speed and turning rate
// double v_d = 0.0;     // [m/s]
// double omega_d = 0.0; // [rad/s]

// // Variable to store desired wheel speeds [m/s]
// double v_Ld = 0.0;
// double v_Rd = 0.0;

// // Counters for milliseconds during interval
// long t_now = 0;
// long t_last = 0;

// // Variables to store errors for controller
// double e_L = 0.0;
// double e_R = 0.0;
// double e_Lint = 0.0;
// double e_Rint = 0.0;

// const double KP = 150.0; // Proportional gain
// const double KI = 200.0; // Integral gain

// const double T = 0.1; //[s]

void setup()
{
    setupMotors();
    Serial.begin(115200);
    Serial.println("Testing motors...");

    moveForward(150);
    delay(3000);
    stop();
}

void loop()
{
    int frontDistance, leftDistance, rightDistance;

    readirData(frontDistance, leftDistance, rightDistance);

    if (threshold_distance > frontDistance || threshold_distance > leftDistance || threshold_distance > rightDistance)
    {
        stop();
        delay(5000);
    }

    t_now = millis();

    // Perform control update every T milliseconds
    if (t_now - t_last >= T)
    {
        StaticJsonDocument<1024> setSpeed;
        do
        {
            String desiredVelocity = Serial.readStringUntil('\n');
            desiredVelocity.trim();


            DeserializationError error = deserializeJson(setSpeed, desiredVelocity);
            if (error)
            {
                Serial.print(F("deserializeJson() failed: "));
                Serial.println(error.c_str());
                return;
            }
        }
        // type == 0 for sending pi to arduino
        while (setSpeed["type"].as<float>() != 0);
        
        Serial.println(setSpeed["trans_speed"].as<float>());
        Serial.println(setSpeed["angular_speed"].as<float>());
        Serial.println(setSpeed["type"].as<float>());

        // Set the desired vehicle speed and turning rate
        v_d = 0.5;     // [m/s]
        omega_d = 0.0; // [rad/s]

        // Estimate the rotational speed of each wheel [rad/s]
        omega_L = compute_wheel_rate(encoder_ticks_L, (double)(t_now - t_last));
        omega_R = compute_wheel_rate(encoder_ticks_R, (double)(t_now - t_last));

        // Compute the speed of each wheel [m/s]
        v_L = compute_wheel_speed(omega_L);
        v_R = compute_wheel_speed(omega_R);

        // Compute the speed of the vehicle [m/s]
        v = compute_vehicle_speed(v_L, v_R);

        // Compute the turning rate of the vehicle [rad/s]
        omega = compute_vehicle_rate(v_L, v_R);

        // Record the current time [ms]
        t_last = t_now;

        // Reset the encoder ticks counter
        encoder_ticks_L = 0;
        encoder_ticks_R = 0;

        // Compute the desired wheel speeds from v_d and omega_d
        v_Ld = compute_L_wheel_speed(v_d, omega_d);
        v_Rd = compute_R_wheel_speed(v_d, omega_d);

        // Compute errors
        e_L = v_Ld - v_L;
        e_R = v_Rd - v_R;

        // Integrate errors with anti-windup
        if (abs(u_L) < 255)
        {
            e_Lint += e_L;
        }
        if (abs(u_R) < 255)
        {
            e_Rint += e_R;
        }

        // Compute control signals using PI controller
        u_L = PI_controller(e_L, e_Lint, KP, KI);
        u_R = PI_controller(e_R, e_Rint, KP, KI);

        // Drive the vehicle
        driveVehicle(u_L, u_R);

        // Print some stuff to the serial monitor (or plotter)
        Serial.print("Vehicle_speed_[m/s]:");
        Serial.print(v);
        Serial.print(",");
        Serial.print("Turning_rate_[rad/s]:");
        Serial.print(omega);
        Serial.print(",");
        Serial.print("u_L:");
        Serial.print(u_L);
        Serial.print(",");
        Serial.print("u_R:");
        Serial.print(u_R);
        Serial.print("\n");
    }
}
