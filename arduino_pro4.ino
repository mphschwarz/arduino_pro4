#include <Arduino.h>

//some shit
void setup() {
    Serial.begin(9600);
}

void loop() {
    Serial.write("hi\n");
    delay(1000);
}

void requestManifest(){

}

int getClosestBeacon(){
    return 0;
}