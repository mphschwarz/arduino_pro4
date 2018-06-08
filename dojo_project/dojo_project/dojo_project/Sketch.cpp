﻿#include "include/Dojo.h"

SoftwareSerial mySerial(0,1);	//usual RX,TX
SoftwareSerial BTSerial(2,3);	//(RX | TX) - PINS

// state init
void setup()
{
	// all Baudrates to 9600 of UART-Connection
	mySerial.begin(9600);
	BTSerial.begin(9600);	
	// Mux- and Vibro Ports to output
	DDRC = 0xff;	
	// For WTV setting ports individual
	DDRB |= 0b00111011;	
	// buttons as inputs
	DDRD |= 0b00010000;
	multiplexController(FTDI_PATH);	
	pinSetupWTV();		
	//Setup LED Power on
	digitalWrite(STATUS_LED, LOW);
	// get first response from the hm-11
	firstResponse();
}

void dojo_statemachine(void)
{
	static enum State state = SCAN;
	
	switch (state)
	{
	case SCAN:
		unsigned int ibeacon;
		unsigned int buffer = scan();
		if (buffer != 0)
		{
			ibeacon = buffer;
			mySerial.println(ibeacon);
			// Hier den ibeacon für in den Play-State übergeben!
		}
// 		mySerial.println(buffer);
// 		mySerial.println("checked!");
		break;
	case SD:
		//TODO: what terminates sd loading mode?
	case SERIAL:
		serialCom();
		break;
	}
}

void loop()
{
 	dojo_statemachine();
}
