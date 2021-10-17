#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>
#include <Arduino.h>
#include "PID.h"

/**
 * Uses PID control to drive test stand orientation to a setpoint
 **/

// Ziegler-Nichols Method constants
float Ku = 0.5;      // ultimate gain Ku
float Tu = 12.35 / 4.0;     // (s)
float Ti = 0.5 * Tu;    //(s)
float Td = 0.125 * Tu;  //(s)


using reaction_wheels_pid::PID;

// ESC & motor setup
Servo esc;
int escPin = 2;
float steadySpeed = 30;  // "center" speed

// PID setup
float Kp = 0.4;
float Ki = 0;
float Kd = 1.0;
float input, output, setPoint;
PID pid(input, output, setPoint, Kp, Ki, Kd);
float writeValue;  // layer of separation from PID output to give more control (ex: clamping the ESC command)
float target_angle = 0;

// IMU setup
/* Set the delay between fresh samples */
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;

// Check I2C device address and correct line below (by default address is 0x29 or 0x28)
//                                   id, address
Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);

// Writes the commanded value to the ESC for the duration (milliseconds)
void ESC_write(Servo esc, int value, int duration);

void setup() {
    Serial.begin(9600);
    Serial.println("Starting Initialization"); Serial.println("");

    /* Initialise the sensor */
    if (!bno.begin())
    {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1);
    }

    // ESC setup
    esc.attach(escPin, 1000, 2000);
    // write 0 to ESC for initialization
    ESC_write(esc, 0, 5000);

    // Spin up motor to arbitrary initial speed for 10 seconds.
    writeValue = steadySpeed;
    ESC_write(esc, writeValue, 10000);

    // Initialize input and start PID controller
    sensors_event_t orientationData;  // To store orientation from IMU
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    float angle = orientationData.orientation.x;
    if (angle > 180) {
        angle = angle - 360;
    }

    // PID initialization
    setPoint = target_angle;
    output = 0;
    input = angle;
    pid.Initialize();

    Serial.println("Starting loop in 3 seconds");
    delay(3000);
}

void loop() {
    // Refresh input from IMU
    sensors_event_t orientationData;  // To store orientation from IMU
    bno.getEvent(&orientationData, Adafruit_BNO055::VECTOR_EULER);
    float angle = orientationData.orientation.x;

    if (angle > 180) {
        angle = angle - 360;
    }
    input = angle;

    pid.Iterate();
    writeValue = output + steadySpeed;

    // clamp write value to reasonable range
    if (writeValue > 50) {
        writeValue = 50;
    }
    else if (writeValue < 10) {
        writeValue = 10;
    }
    esc.write(writeValue);

    Serial.print("angle: ");
    Serial.println(input);
    Serial.print("command: ");
    Serial.println(writeValue);
    Serial.print("error: ");
    Serial.println(setPoint - input);
}


void ESC_write(Servo esc, int value, int duration) {
    int starttime = millis();
    int endtime = starttime;
    while (endtime - starttime < duration) {
        esc.write(value);
        endtime = millis();
    }
}