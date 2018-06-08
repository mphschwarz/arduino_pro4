
#include <Arduino.h>

#include <SoftwareSerial.h>
#include <Time.h>

SoftwareSerial BTSerial(2,3); //(RX | TX) - PINS

#define VIBRO PORTC
#define VIBRO_ON (1<<5)

void scan();
void sendCommand(int com);
int readAnswer();
int saveBeacon(int i);
void filt(int answerByte);
void rssiCompare();
void firstResponse();
int saveBeacon(int i);

struct UUID_RSSI_values
{
	char UUID[32];
	char value_RSSI[3];
};

struct UUID_RSSI_values getUUID_RSSI();
void printBeacons(UUID_RSSI_values beacon);

int incomingByte = 0;
char command_AT[2] = {'A','T'};
char command_ATDISI[8] = {'A','T','+','D','I','S','I','?'};
char endString[8];

UUID_RSSI_values beacon1;
UUID_RSSI_values beacon2;
UUID_RSSI_values beacon3;
UUID_RSSI_values beacon4;
UUID_RSSI_values beacon5;
UUID_RSSI_values beacon6;

void setup() 
{
  Serial.begin(9600);
  BTSerial.begin(9600);  
    
  DDRC = 255;        //set all PINS to output
  //VIBRO = (1<<5);         //test VIBRO as LED
  
  //send AT until OK response from BT-Module
  firstResponse();
}

void firstResponse()
{
	while(!BTSerial.available())
	{
		sendCommand(1);
		delay(2500);
	}
	
	while (1)
	{
		if (readAnswer() == 79)
		{
			if (readAnswer() == 75)
			{
				break;
			}
			else
			{
				firstResponse();
			}
		}
		else
		{
			firstResponse();
		}
	}		
}

void scan()
{
	int countBeacon = 1;
	sendCommand(2);
	int mischa_koeffizient = 0;
	
	while(1)
	{
		mischa_koeffizient = readAnswer();
			
		if(mischa_koeffizient == 58)
		{
			boolean state_zero = true;
       
			for(int i = 0; i < 8; i++)			//search for !=0 in FactoryID
			{         
				if(readAnswer() != 48)
				{
					state_zero = false;
				}
			}
			if(state_zero == false)                             
			{
				state_zero = true;
				countBeacon = saveBeacon(countBeacon);
				continue;
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
		else if (mischa_koeffizient == 69)
		{
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

void filt(int answerByte)
{
  
}

int saveBeacon(int i)
{
	switch(i)
	{
		case 1:
			beacon1 = getUUID_RSSI();
			printBeacons(beacon1);
			break;
		case 2:
			beacon2 = getUUID_RSSI();
			printBeacons(beacon2);
			break;
		case 3:
			beacon3 = getUUID_RSSI();
			printBeacons(beacon3);
			break;
		case 4:
			beacon4 = getUUID_RSSI();
			printBeacons(beacon4);
			break;
		case 5:
			beacon5 = getUUID_RSSI();
			printBeacons(beacon5);
			break;
		case 6:
			beacon6 = getUUID_RSSI();
			printBeacons(beacon6);
			break;
		default:
			//do nothing;
			break;
	}
	return i++;
}

struct UUID_RSSI_values getUUID_RSSI()
{
	UUID_RSSI_values beacon;
	
	while(readAnswer() != 58)
	{/*wait until ':' is recived*/}
	for(int i = 0; i < 32; i++)
	{
		beacon.UUID[i] = readAnswer();
	}
	while(readAnswer() != 45)
	{/*wait until '-' is recived*/}
	for(int i = 0; i < 3; i++)
	{
		beacon.value_RSSI[i] = readAnswer();
	}
	return beacon;
}

void printBeacons(UUID_RSSI_values beacon)
{	
	Serial.println("Beacon:");
	Serial.print("UUID: ");
	for(int i = 0; i < 32; i++)				//write values to emulator
	{
		Serial.write(beacon.UUID[i]);
	}
	Serial.println("");
	Serial.print("RSSI: "); Serial.print("-");
	for(int i = 0; i < 3; i++)
	{
		Serial.write(beacon.value_RSSI[i]);			//end write values to emulator
	}
	Serial.println("dbm");
}

void rssiCompare()
{
  
}

void loop()
{
	Serial.println("Start scanning...");
	scan();
	Serial.println("Finished scanning!");
	delay(4000);
}