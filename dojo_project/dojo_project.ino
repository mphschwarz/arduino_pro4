
#define LEDPORT PORTD
#define LEDDDR DDRD

//int ledPin = A0;
void setup() {
  LEDDDR = 0xFF;
}

void loop() {
LEDPORT = 0xFF;
_delay_ms(500);
LEDPORT = 0x00;
_delay_ms(500);
}
