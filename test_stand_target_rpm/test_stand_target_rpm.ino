#include <Servo.h>
#include <Arduino.h>
#include "PID.h"

/**
 * Uses PID control to drive flywheel RPM to a setpoint
 **/

using reaction_wheels_pid::PID;

// ESC & motor setup
Servo esc;
int escPin = 2;

// RPM monitor setup
int monitorPin = 3;
short sensorReading;
volatile unsigned long lastRotTime;
volatile unsigned long lastRotTimeLinear;  // to use in linear deceleration estimate
volatile int pulseCount;
float rpm;

const int POLES = 14;  // number of poles in the motor
const float MILTOMIN = 60000.0;  // conversion mins to millis
const int ROTS_TO_COUNT = 3;  // in how many rotations to check the rpm value
volatile bool checkRPM;

const long DEBOUNCE_TIME = 10;  // debouncing time in milliseconds
volatile unsigned long last_micros;  // timestamp of last sensor reading

// PID setup
float Kp = 0.5;
float Ki = 0.01;
float Kd = 0.1;
float input, output, setPoint;
PID pid(input, output, setPoint, Kp, Ki, Kd);
float writeValue;  // layer of separation from PID output to give more control (ex: clamping the ESC command)
float target_rpm;


int KpPin = 0;


// Writes the commanded value to the ESC for the duration (milliseconds)
void ESC_write(Servo esc, int value, int duration);

// Calculate new RPM
void rpm_calculate();


void setup() {
    Serial.begin(9600);

    // ESC setup
    esc.attach(escPin, 1000, 2000);
    // write 0 to ESC for initialization
    ESC_write(esc, 0, 5000);

    // RPM monitor interrupt setup
    attachInterrupt(digitalPinToInterrupt(monitorPin), debounceMonitor, RISING);
    lastRotTime = millis();
    pulseCount = 0;
    rpm = 0;
    last_micros = micros();

    // PID initialization
    setPoint = 450;  // drive flywheel to 350 rpm
    output = 0;

    // Spin up motor to arbitrary initial speed.
    writeValue = 25;
    ESC_write(esc, writeValue, 10000);

    // enable interrupts
    sei();

    // initialize RPM
    rpm = 0;
    int starttime = millis();
    int endtime = starttime;
    while (endtime - starttime < 10000) {
        if (checkRPM) {
            rpm_calculate();
            checkRPM = false;
        }
        endtime = millis();
    }

    cli();
    // initialize PID controller
    pid.Initialize();
    sei();
}

void loop() {
    // control Kp with potentiometer
    Kp = (float) analogRead(KpPin) / 512;  // map 0-1023 to 0-2

    // RPM calculations with interrupts disabled
    if (checkRPM) {
        cli();
        rpm_calculate();
        checkRPM = false;
        sei();
    }

    // If no pulse has come through in more than threshold, rpm = 0
    // TODO: Maybe instead of setting to 0, use a linear estimate of rpm based on time since last pulse
    // TODO: Despins from 400 rpm to 0 in 5.8 seconds
    if (millis() - lastRotTimeLinear > 1000) {
        cli();
        rpm -= 66;  // flywheel decelerates at 66 rpm/s
        if (rpm < 0) {
            rpm = 0;
        }

        // rpm = 0;
        Serial.println("RPM ZERO");
        lastRotTimeLinear = millis();
        sei();
    }

    // PID iteration
    input = rpm;
    pid.Iterate();
    writeValue = output;
    // clamp write value to reasonable range
    if (writeValue > 60) {
        writeValue = 60;
    }
    else if (writeValue < 0) {
        writeValue = 0;
    }
    esc.write(writeValue);

    Serial.print("Kp: ");
    Serial.println(pid.get_Kp());
    Serial.print("rpm: ");
    Serial.println(rpm);
    Serial.print("command: ");
    Serial.println(writeValue);
    Serial.print("error: ");
    Serial.println(setPoint - input);
}

void rpm_calculate() {
    unsigned long currentTime = millis();
    unsigned long elapsedTime = currentTime - lastRotTime;
    rpm = MILTOMIN / ((float) elapsedTime / ROTS_TO_COUNT);

    // Serial.print("current time: ");
    // Serial.println(currentTime);
    // Serial.print("last time: ");
    // Serial.println(lastRotTime);
    // Serial.print("elapsed time: ");
    // Serial.println(elapsedTime);

    lastRotTime = currentTime;
    lastRotTimeLinear = currentTime;
}

// RPM monitor interrupt
// simple software debounce of monitor()
void debounceMonitor() {
    if ((long) (micros() - last_micros) >= DEBOUNCE_TIME * 1000) {
        monitor();
        last_micros = micros();
    }
}
// counts pulses sent by RPM monitor
void monitor() {
    cli();
    pulseCount++;
    if (pulseCount == POLES * ROTS_TO_COUNT) {
        checkRPM = true;
        pulseCount = 0;
    }
    sei();
}

void ESC_write(Servo esc, int value, int duration) {
    int starttime = millis();
    int endtime = starttime;
    while (endtime - starttime < duration) {
        esc.write(value);
        endtime = millis();
    }
}