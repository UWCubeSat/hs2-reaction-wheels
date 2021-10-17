#include <Servo.h>
#include <avr/interrupt.h>
#include <avr/io.h>

Servo esc;
int escPin = 2;
int throttlePin = 0;
int throttle;

int monitorPin = 3;
short sensorReading;
volatile unsigned long lastRotTime;
volatile int pulseCount;
float rpm;

const int POLES = 14;  // number of poles in the motor
const float MILTOMIN = 60000.0;  // conversion mins to millis
const int ROTS_TO_COUNT = 3;  // in how many rotations to check the rpm value
volatile bool checkRPM;

const long DEBOUNCE_TIME = 10;  // debouncing time in milliseconds
volatile unsigned long last_micros;  // timestamp of last sensor reading

void setup() {
    Serial.begin(9600);
    esc.attach(escPin, 1000, 2000);

    // set up interrupt on rpm monitor pin
    attachInterrupt(digitalPinToInterrupt(monitorPin), debounceMonitor, RISING);

    lastRotTime = millis();
    pulseCount = 0;
    rpm = 0;

    last_micros = micros();

    // enable interrupts
    sei();
}

void loop() {
    throttle = analogRead(throttlePin);
    throttle = map(throttle, 0, 1023, 0, 50);
    esc.write(throttle);

    cli();
    if (checkRPM) {
        unsigned long currentTime = millis();
        unsigned long elapsedTime = currentTime - lastRotTime;
        rpm = MILTOMIN / ((float) elapsedTime / ROTS_TO_COUNT);

        // Serial.println("loop");

        Serial.print("current time: ");
        Serial.println(currentTime);
        Serial.print("last time: ");
        Serial.println(lastRotTime);
        Serial.print("elapsed time: ");
        Serial.println(elapsedTime);
        Serial.print("rpm: ");
        Serial.println(rpm);

        lastRotTime = currentTime;
        checkRPM = false;
    }
    sei();
}

void debounceMonitor() {
    if ((long) (micros() - last_micros) >= DEBOUNCE_TIME * 1000) {
        monitor();
        last_micros = micros();
    }
}

void monitor() {
    cli();
    pulseCount++;
    if (pulseCount == POLES * ROTS_TO_COUNT) {
        checkRPM = true;
        pulseCount = 0;
    }
    sei();
}