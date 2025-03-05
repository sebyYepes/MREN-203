#include <Arduino_LSM6DS3.h>

// Variables to store angular rates from the gyro [degrees/s]
float omega_x, omega_y, omega_z;

// Variables to store accelerations [g's]
float a_x, a_y, a_z;

// Variables to store sample rates from sensor [Hz]
float a_f, g_f;

// Gyroscope bias values
float gyro_bias_x = 0.0, gyro_bias_y = 0.0, gyro_bias_z = 0.0;

void setup()
{
    // Open the serial port
    Serial.begin(9600);

    // Wait for serial connection before starting
    while (!Serial)
    {
        delay(10);
    }

    Serial.println();

    // Initialize the IMU
    if (!IMU.begin())
    {
        Serial.println("Failed to initialize IMU :(");
        while (1)
        {
            delay(10);
        }
    }

    // Read the sample rates
    a_f = IMU.accelerationSampleRate();
    g_f = IMU.gyroscopeSampleRate();

    Serial.print("Accelerometer sample rate: ");
    Serial.println(a_f);
    Serial.print("Gyroscope sample rate: ");
    Serial.println(g_f);

    // Calibrate the gyroscope
    calibrateGyroBias();
}

void loop()
{
    // Read from the accelerometer
    if (IMU.accelerationAvailable())
    {
        IMU.readAcceleration(a_x, a_y, a_z);

        // Print the accelerometer measurements
        Serial.print(a_x);
        Serial.print("\t");
        Serial.print(a_y);
        Serial.print("\t");
        Serial.print(a_z);
        Serial.print(" g\t\t");
    }

    // Read from the gyroscope
    if (IMU.gyroscopeAvailable())
    {
        IMU.readGyroscope(omega_x, omega_y, omega_z);

        // Subtract the bias from the gyro readings
        omega_x -= gyro_bias_x;
        omega_y -= gyro_bias_y;
        omega_z -= gyro_bias_z;

        // Print the corrected gyroscope measurements
        Serial.print(omega_x);
        Serial.print("\t");
        Serial.print(omega_y);
        Serial.print("\t");
        Serial.print(omega_z);
        Serial.println(" deg/s");
    }
}

// Function to calibrate the gyroscope bias
void calibrateGyroBias()
{
    const int num_samples = 500; // Number of samples to average
    float sum_x = 0.0, sum_y = 0.0, sum_z = 0.0;

    Serial.println("Calibrating gyro bias. Keep the IMU stationary...");

    // Collect multiple samples
    for (int i = 0; i < num_samples; i++)
    {
        if (IMU.gyroscopeAvailable())
        {
            float gx, gy, gz;
            IMU.readGyroscope(gx, gy, gz);
            sum_x += gx;
            sum_y += gy;
            sum_z += gz;
        }
        delay(10); // Adjust delay to match the sample rate
    }

    // Calculate the average bias for each axis
    gyro_bias_x = sum_x / num_samples;
    gyro_bias_y = sum_y / num_samples;
    gyro_bias_z = sum_z / num_samples;

    Serial.println("Gyro bias calibration complete:");
    Serial.print("Bias X: ");
    Serial.println(gyro_bias_x);
    Serial.print("Bias Y: ");
    Serial.println(gyro_bias_y);
    Serial.print("Bias Z: ");
    Serial.println(gyro_bias_z);
}
