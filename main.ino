#include <Servo.h>
#include <Wire.h>
#include <LSM6.h>
#include "PID.h"

using reaction_wheels_pid::PID;

LSM6 imu;
float imuRestAvg;
Servo esc;

float Kp = 0.0010;
float Ki = 0;
float Kd = 0;
float input, output, setPoint;
PID pid(input, output, setPoint, Kp, Ki, Kd);

float writeValue;

void ESC_write(Servo esc, int value, int duration);

void setup() {
    Serial.begin(9600);
    Serial.println("Beginning initialization");

    // ESC initialization
    esc.attach(10, 1000, 2000);
    ESC_write(esc, 0, 6000);  // write 0 to the ESC for startup

    delay(3000);

    // IMU initialization
    Wire.begin();
    if (!imu.init()) {
        Serial.println("Failed to detect and initialize IMU!");
        while (1);
    }
    imu.enableDefault();

    imuRestAvg = 0;
    // Calibrate imu output at rest
    for (int i = 0; i < 1000; ++i) {
        imu.read();
        imuRestAvg += imu.g.z;
    }
    imuRestAvg /= 1000;

    // PID initialization
    setPoint = imuRestAvg;  // drive rotational speed to 0
    output = 0;
    writeValue = 25;
    // Spin up motor to store momentum.
    // The stand's rotation should be manually set
    // to the desired test speed during this process.
    ESC_write(esc, writeValue, 10000);
    imu.read();
    input = imu.g.z;
    pid.Initialize();

    delay(3000);

    Serial.println("Initialized; beginning iteration");
}

void loop() {
    imu.read();
    input = imu.g.z;
    //Serial.println(output + 25);
    Serial.println(setPoint - input);

    writeValue += output;

    if (writeValue > 40) {
        writeValue = 40;
    }
    else if (writeValue < 0) {
        writeValue = 0;
    }
    esc.write(writeValue);

    pid.Iterate();
    delay(100);
}

// Writes the value (PWM) to the ESC for the duration (milliseconds)
void ESC_write(Servo esc, int value, int duration) {
    int starttime = millis();
    int endtime = starttime;
    while (endtime - starttime < duration) {
        esc.write(value);
        endtime = millis();
    }
}