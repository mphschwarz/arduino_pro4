#include <SoftwareSerial.h>

SoftwareSerial gtSerial(2, 3); // Arduino RX, Arduino TX

void setup() {
  Serial.begin(9600);    // serial / USB port
  gtSerial.begin(9600);  // software serial port
}

byte rx_byte = 0;        // stores received byte
boolean start = true;
int a = 0;




void loop() {
  a = getID();
}



int getID(){
  //AT+DISI? senden
  if (start == true) {
    gtSerial.write("AT+DISI?");              //AT DISI send
    start = false;
  }

  if (gtSerial.available()) {             // check if byte available on the software serial port
    rx_byte = gtSerial.read();            // get the byte from the software serial port

    if (rx_byte == 58 || rx_byte == 43) {   //check for + und :
      Serial.println("");
    }

    Serial.write(rx_byte);

  }
}


