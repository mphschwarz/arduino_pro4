#include <SoftwareSerial.h>
#include <Time.h>

//(RXD | TXD) numeric pins on the arduino board
SoftwareSerial BTSerial (8, 9);

String inputString = "";
boolean stringComplete = false;

int lastMinute = -1;

void setup()
{
  Serial.begin(9600);
  BTSerial.begin(9600);

  inputString.reserve(200);

  Serial.println("Starting");

  //send setup bluetooth commands
  at("AT"); // check if working, always returns OK
  at("AT+ROLE1"); // select master = central
  at("AT+RESET"); // actually more a restart than a reset .. needed after ROLE
  at("AT+SHOW1"); // include Bluetooth name in response
  //at("AT+FILT0"); // show all BLE devices, not only HM ones
  delay(1000); // wait a bit, NECESSARY!!
  at("AT+IMME1"); // works immediatly when power is on!
}

void at(char* cmd)
{
  BTSerial.write(cmd);
  while (!BTSerial.find("OK"))
  {}
}

void processString(String str)
{
  String deviceID = "";
  String deviceName = "";

  int startID = str.indexOf("+DISC1:") + 6;
  int endID = str.indexOf("+NAME:");
  int startName = endID + 6;
  int endName = str.indexOf("\n");

  deviceID = str.substring(startID, endID);
  deviceName = str.substring(startName, endName);

  Serial.println(deviceID);
  Serial.println(deviceName);
}

void scan()
{
  Serial.println("Scanning...");
  at("AT+DISC?");
}

void checkBTResponse()
{
  char inChar;
  while (BTSerial.available())
  {
    inChar = (char)BTSerial.read();
    inputString += inChar;
    if (inChar == '\n')
    {
      stringComplete = true;
    }
  }
  if (stringComplete == true)
  {
    processString(inputString);
    stringComplete = false;
    inputString = "";
  }
}

void timer()
{
  delay(10000);
  scan();
}

void loop()
{
  timer();
  checkBTResponse();
}

