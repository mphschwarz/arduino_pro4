#include <SoftwareSerial.h>

SoftwareSerial BTSerial (8,9); //RXD | TXD

void setup()
{
  BTSerial.begin(9600);
  Serial.begin(9600);
  delay(100);
  Serial.println("Enter AT-Commands:");
}

void loop()
{
  if(Serial.available() > 0)          
  {
    BTSerial.write(Serial.read());    //writes from arduino (serial monitor) to HM-10
  }
  if(BTSerial.available() > 0)        
  {
    Serial.write(BTSerial.read());    //writes response from HM-10 to arduino (serial monitor)
  }
}

