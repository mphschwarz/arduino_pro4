#include <Arduino.h>

void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.write("hi\n");
    delay(1000);
}

int getClosestBeacon(){
    return 0;
}