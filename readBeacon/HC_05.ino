#include <SoftwareSerial.h>

SoftwareSerial BTSerial(10, 11); // RX | TX

void setup() 
{
  Serial.begin(38400);
  Serial.println("Enter AT commands:");
  BTSerial.begin(38400);  // HC-05 default speed in AT command more
  pinMode(9, OUTPUT);  // this pin will pull the HC-05 pin 34 (key pin) HIGH to switch module to AT mode
  digitalWrite(9, HIGH); 
  BTSerial.println("AT");
  BTSerial.println("AT+NAME=Data_Analyzer");
  BTSerial.println("AT+NAME?");
  BTSerial.println("AT+PSWD=2017");
  BTSerial.println("AT+PSWD?");
  BTSerial.println("AT+UART=9600,0,0");
  BTSerial.println("AT+UART?");
}

void loop()
{

  // Keep reading from HC-05 and send to Arduino Serial Monitor
  if (BTSerial.available())
  {
      Serial.write(BTSerial.read());
  }
  // Keep reading from Arduino Serial Monitor and send to HC-05
  if (Serial.available())
  {
    BTSerial.write(Serial.read());
   }
}


