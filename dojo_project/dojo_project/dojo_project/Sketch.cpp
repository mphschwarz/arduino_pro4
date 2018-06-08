#include "include/Dojo.h"

SoftwareSerial mySerial(0,1);	//usual RX,TX
SoftwareSerial BTSerial(2,3);	//(RX | TX) - PINS

void setup()
{
	mySerial.begin(9600);
	BTSerial.begin(9600);
	
	DDRC = 0xff;				//set all PINS to output
	//DDRD = 0xff;
	DDRB = 0xff;
	
	PORTC = 0b00011010;
	PORTB = 0b00000000;
	
	pinSetupWTV();
	
	pinMode(4, OUTPUT);
	
	//Setup LED Power on
	digitalWrite(PORTD4, LOW);
}

void loop()
{
	
}