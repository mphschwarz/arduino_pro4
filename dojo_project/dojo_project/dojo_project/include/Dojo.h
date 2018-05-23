/*
 * Dojo.h
 *
 * Created: 23.05.2018 14:45:15
 *  Author: Admin
 */ 
#ifndef DOJO_H_
#include <Arduino.h>
#endif DOJO_H_

#define MULTIPLEXER_PORT	PORTC
#define SD_PATH				0b00011010		//just default!!
#define MC_PATH				0b00000101
#define VIBRO_ON_OFF		0b00100000
#define FTDI_PATH			0b00000000		//from MCU over FTDI to USB micro B to computer
#define UUID_ARRAY_LENGTH	3
#define RSSI_ARRAY_LENGTH	3
#define MAJOR_ARRAY_LENGTH	4
#define FILTER_MAJOR_VALUE	3				//to filter other beacons
#define RSSI_STRENGTH		90				//to filter beacon with a too big distance

#define WTV_DOUT			12
#define WTV_CLK				13
#define WTV_RESET			9

#define OPCODE_PLAY_PAUSE	0xFFFE			//play track or stop the current, first send track number 0-511
#define OPCODE_STOP			0xFFFF			//stops the current playback
#define OPCODE_VOL			0xFFF0			//0xFFF0 is muted 0xFFF7 is max volume

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
void sendWTVcommand(unsigned int command);
void resetWTV();
void pinSetupWTV();


/*#ifndef DOJO_H_
#define DOJO_H_





#endif /* DOJO_H_ */