/*
 * Uses a Kalman Filter to estimate the orientation of the IMU. 
 */ 

#include <SimpleKalmanFilter.h>
#include <Wire.h>
#include <LSM6.h>

LSM6 imu;
const float IMU_TO_DEG_S = 0.004375;
float imuRestAvg;
float measured_rate;
float measured_pos;
float estimated_rate;
float estimated_pos;
float time_prev;

//! update error estimates based on imu_test.ino
SimpleKalmanFilter simpleKalmanFilter(1, 1, 0.01);

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;

void setup() {
    Serial.begin(115200);
    Serial.println("Beginning sensing");

    Wire.begin();
    if (!imu.init()) {
        Serial.println("Failed to detect and initialize IMU!");
        while (1);
    }

    imu.enableDefault();
    // Calibrate imu output at rest
    imuRestAvg = 0;
    for (int i = 0; i < 1000; ++i) {
        imu.read();
        imuRestAvg += imu.g.z * IMU_TO_DEG_S;
    }
    imuRestAvg /= 1000;

    time_prev = (float) micros() / 1000000;
    refresh_time = millis() + SERIAL_REFRESH_TIME;

    measured_pos = 0;
    estimated_pos = 0;
}

void loop() {
    float time_curr = (float) micros() / 1000000;
    float dt = time_curr - time_prev;

    imu.read();
    measured_rate = imu.g.z * IMU_TO_DEG_S - imuRestAvg;
    measured_pos += measured_rate * dt;
    //estimated_rate = simpleKalmanFilter.updateEstimate(measured_rate);
    //estimated_pos += estimated_rate * dt;
    estimated_pos = simpleKalmanFilter.updateEstimate(measured_pos);

    // Print every 100 ms
    if (millis() > refresh_time) {
        Serial.print("Estimated position: ");
        Serial.print(estimated_pos);
        Serial.print(" deg at measured rate ");
        Serial.print(measured_rate);
        // Serial.print(" and estimated rate ");
        // Serial.println(estimated_rate);
        Serial.print(" and measured posistion ");
        Serial.println(measured_pos);
        refresh_time = millis() + SERIAL_REFRESH_TIME;
    }

    time_prev = time_curr;
}