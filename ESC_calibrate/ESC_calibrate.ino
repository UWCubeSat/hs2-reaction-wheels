#include <Servo.h>

Servo esc;
void setup() {
    Serial.begin(9600);
    esc.attach(2, 1000, 2000);
    Serial.println("Starting ESC calibration");
    int starttime = millis();
    int endtime = starttime;
    while (endtime - starttime < 5000) {
        esc.write(180);
        endtime = millis();
    }
    starttime = millis();
    endtime = starttime;
    Serial.println("Sending minimum value");
    while (endtime - starttime < 5000) {
        esc.write(0);
        endtime = millis();
    }
    Serial.println("ESC calibration done");
}

void loop() {
    
}