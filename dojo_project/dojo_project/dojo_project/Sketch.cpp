
#include <Arduino.h>
#include <SoftwareSerial.h>
#define LEDPORT PORTD
#define LEDDDR DDRD

SoftwareSerial mySerial = SoftwareSerial(0,1);
void setup() {
  LEDDDR = 0xFF;
}

void loop() {
// LED 
LEDPORT = 0xFF;
_delay_ms(500);
LEDPORT = 0x00;
_delay_ms(500);
}
