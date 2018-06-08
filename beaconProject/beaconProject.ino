#include <SoftwareSerial.h>
#include <Time.h>

SoftwareSerial BTSerial(2,3); //(RX | TX) - PINS

#define VIBRO PORTC
#define VIBRO_ON (1<<5)

int incomingByte = 0;
char UUID[32];
char value_RSSI[3];
char command_AT[2] = {'A','T'};
char command_ATDISI[8] = {'A','T','+','D','I','S','I','?'};
boolean scanned = false;
boolean UUID_RSSI = false;

void setup() 
{
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Start:"); 
  BTSerial.begin(9600);  
    
  DDRC = 255;        //set all PINS to output
  //VIBRO = (1<<5);         //test VIBRO as LED
  
  //send dummy string
  BTSerial.print("asdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdfasdf");     
  delay(300);
}

void scan()
{
  sendCommand(2);
  while(1)
  {
    //Serial.write(readAnswer());
    if(readAnswer() == 58)
    {
       boolean state_zero = true;
       
       for(int i = 0; i < 8; i++)
       {         
          if(readAnswer() != 0)
          {
            state_zero = false;
          }
       }
       if(state_zero == false)                              //ebis lauft do nonig wies sett!!
       {
          Serial.println("ok");
          state_zero = true;
          getUUID_RSSI();
       }
       else
       {
          for(int i = 0; i < 4; i++)
          {
            while(readAnswer() != 58)
            {/*wait until ':' is recived*/}           
          }
          continue;
       }
    }
    if(UUID_RSSI == true)
    {
      UUID_RSSI = false;
      for(int i = 0; i < 32; i++)
      {
        Serial.write(UUID[i]);
      }
      Serial.println("");
      for(int i = 0; i < 3; i++)
      {
        Serial.write(value_RSSI[i]);
      }
      while(1){}
    }
    if(scanned == true)
    {
      scanned = false;
      break;
    }
  }
}

void sendCommand(int com)
{
  switch(com)
  {
    case 1:
      for(int i = 0; i < 2; i++)
      {
        BTSerial.write(command_AT[i]);
      }
      break;
    case 2:
      for(int i = 0; i < 8; i++)
      {
        BTSerial.write(command_ATDISI[i]);
      }
      break;
    default:
      //do nothing
      break;
  }
}

int readAnswer()
{
  while(!BTSerial.available())
  {/*wait until datas are available to read*/}  
  incomingByte = BTSerial.read();
  
  return incomingByte;
}

void filtID(int answerByte)
{
  
}

void getUUID_RSSI()
{
  while(readAnswer() != 58)
  {/*wait until ':' is recived*/}
  for(int i = 0; i < 32; i++)
  {
    UUID[i] = readAnswer();
  }
  while(readAnswer() != 45)
  {/*wait until '-' is recived*/}
  for(int i = 0; i < 3; i++)
  {
    value_RSSI[i] = readAnswer();
  }
  UUID_RSSI = true;
}

void rssiCompare()
{
  
}

void loop()
{
  scan();
  Serial.println("CHECK");
  delay(10000);
}
