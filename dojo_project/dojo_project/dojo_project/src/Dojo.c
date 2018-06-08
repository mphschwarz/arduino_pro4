/*
 * Dojo.c
 *
 * Created: 23.05.2018 14:48:22
 *  Author: Admin
 */ 

#include "../include/Dojo.h"
#include <SoftwareSerial.h>
#include <Time.h>

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


void firstResponse()
{
	while(!BTSerial.available())
	{
		mySerial.println("do");
		sendCommand(1);
		delay(2500);
	}
	
	while (1)
	{
		if (readAnswer() == 'O')
		{
			if (readAnswer() == 'K')
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
	{/*/wait until ':' is recived/*/}
	beacon.UUID_value = readValueDec(UUID_ARRAY_LENGTH);
	
	while (readAnswer() != ':')
	{/*/wait until ":" is recieved/*/}
	beacon.MAJOR_value = readValueDec(MAJOR_ARRAY_LENGTH);
	
	while(readAnswer() != '-')
	{/*/wait until '-' is recived/*/}
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

void sendWTVcommand(unsigned int command){
	digitalWrite(WTV_CLK, LOW);
	//delayMicroseconds(1900);
	_delay_us(1900);
	for (byte i = 0; i < 16; i++)
	{
		//delayMicroseconds(100);
		_delay_us(100);
		digitalWrite(WTV_CLK, LOW);
		digitalWrite(WTV_DOUT, LOW);
		if ((command & 0x8000) != 0)
		{
			digitalWrite(WTV_DOUT, HIGH);
		}
		//delayMicroseconds(100);
		_delay_us(100);
		digitalWrite(WTV_CLK, HIGH);
		command = command<<1;
	}
}

void resetWTV()
{
	digitalWrite(WTV_RESET, LOW);
	_delay_ms(100);
	digitalWrite(WTV_RESET, HIGH);
	_delay_ms(500);
}

void pinSetupWTV()
{
	pinMode(WTV_RESET, OUTPUT);
	digitalWrite(WTV_RESET, LOW);
	pinMode(WTV_DOUT, OUTPUT);
	digitalWrite(WTV_DOUT, HIGH);
	pinMode(WTV_CLK, OUTPUT);
	digitalWrite(WTV_CLK, HIGH);
}