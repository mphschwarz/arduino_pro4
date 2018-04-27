
#include <Arduino.h>

#include <SoftwareSerial.h>
#include <Time.h>

SoftwareSerial BTSerial(2,3); //(RX | TX) - PINS



#define MULTIPLEXER_PORT	PORTC
#define SD_PATH				0b00011010		//just default!!
#define MC_PATH				0b00000101
#define VIBRO_ON_OFF		0b00100000
#define UUID_ARRAY_LENGTH	3
#define RSSI_ARRAY_LENGTH	3
#define MAJOR_ARRAY_LENGTH	4
#define FILTER_MAJOR_VALUE	3				//to filter other beacons
#define RSSI_STRENGTH		90				//to filter beacon with a too big distance

unsigned int scanClosestBeacon();
void scan();
void sendCommand(int com);
int readAnswer();
// int saveBeacon(int i);
void filt(int answerByte);
void rssiCompare();
void firstResponse();
void compareBeacon();
void multiplexController(int multiplex_path);
void vibroController(boolean state);
unsigned int readValueDec(int array_length);
unsigned int castHexChararrToIntDec(char* array);
void testComparing(unsigned int test);

boolean firstBeacon = false;						//at first there must be a beacon for comparing to
// unsigned int countBeacon = 1;

struct BEACON_VALUES_DEC
{
	unsigned int UUID_value;
	unsigned int RSSI_value;			//its the absolut value of RSSI
	unsigned int MAJOR_value;	
};

BEACON_VALUES_DEC closest_Beacon;

struct BEACON_VALUES_DEC getUUID_RSSI();
void printBeacons(BEACON_VALUES_DEC beacon);

char command_AT[2] = {'A','T'};
char command_ATDISI[8] = {'A','T','+','D','I','S','I','?'};
char endString[8];


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

unsigned int scanClosestBeacon()
{
	int compare_koeff;
	
	sendCommand(2);
	
	while(1)
	{
		compare_koeff = readAnswer();
		
		if(compare_koeff == 58)
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
				compareBeacon();
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
		else if (compare_koeff == 69)
		{
// 			countBeacon = 1;
			break;
		}
	}
	firstBeacon = false;
	return closest_Beacon.UUID_value;
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
	int incomingByte;
	
	while(!BTSerial.available())
	{/*wait until datas are available to read*/}  
	  
	incomingByte = BTSerial.read();
  
	return incomingByte;
}

void filt(int answerByte)
{
  
}

void compareBeacon()
{
	BEACON_VALUES_DEC buff = getUUID_RSSI();
	
	if (buff.MAJOR_value == FILTER_MAJOR_VALUE && buff.RSSI_value <= RSSI_STRENGTH && firstBeacon == false)
	{
		firstBeacon = true;
		closest_Beacon = buff;
	}
	else if (buff.MAJOR_value == FILTER_MAJOR_VALUE && buff.RSSI_value <= RSSI_STRENGTH && closest_Beacon.RSSI_value > buff.RSSI_value && firstBeacon == true)
	{
		closest_Beacon = buff;
	}
}

struct BEACON_VALUES_DEC getUUID_RSSI()
{
	BEACON_VALUES_DEC beacon;
	
	while(readAnswer() != ':')
	{/*wait until ':' is recived*/}
	beacon.UUID_value = readValueDec(UUID_ARRAY_LENGTH);
	
	while (readAnswer() != ':')
	{/*wait until ":" is recieved*/}
	beacon.MAJOR_value = readValueDec(MAJOR_ARRAY_LENGTH);
	
	while(readAnswer() != '-')
	{/*wait until '-' is recived*/}
	beacon.RSSI_value = readValueDec(RSSI_ARRAY_LENGTH);
			
	return beacon;
}

unsigned int readValueDec(int array_length)
{	
	unsigned int value = 0;
	
	for(int i = 0; i < array_length; i++)
	{
		value = (value * 10) + (readAnswer() - '0');
	}
	
	return value;
}

void printBeacons(BEACON_VALUES_DEC beacon)
{	
	Serial.println("Beacon:");
	Serial.print("UUID: ");
	Serial.print(beacon.UUID_value);
	Serial.println("");
	
	Serial.print("MAJOR: ");
	Serial.print(beacon.MAJOR_value);
	Serial.println("");
	
	Serial.print("RSSI: "); 
	Serial.print("-");
	Serial.print(beacon.RSSI_value);
	Serial.println("dbm");
}

void rssiCompare()
{
  
}

void multiplexController(int multiplex_path)
{
	MULTIPLEXER_PORT &= 0;			//set all to 0;
	MULTIPLEXER_PORT |= multiplex_path;
}

void vibroController(boolean state)
{
	if (state == true)					//turns the vibro ON
	{
		MULTIPLEXER_PORT |= VIBRO_ON_OFF;
		delay(500);
		vibroController(false);
	}
	else if(state == false)				//turns the vibro OFF
	{
		MULTIPLEXER_PORT ^= VIBRO_ON_OFF;
	}
}

unsigned int castHexChararrToIntDec(char* array)
{
	char* pEnd;
	unsigned int cast = strtol(array, &pEnd ,16);
	return cast;
}

void testComparing(unsigned int test)
{
	static unsigned int compare = 0;
	
	if (test != compare && test != 0)
	{
		compare = test;
		Serial.println(compare);					//to print first three numbers of UUID
		closest_Beacon.UUID_value = 0;
		closest_Beacon.MAJOR_value = 0;
		closest_Beacon.RSSI_value = 0;
		vibroController(true);
	}
	else 
	{
		//do nothing!
	}
}

void scan()
{
	testComparing(scanClosestBeacon());
}

void loop()
{
	scan();
	delay(2000);
}