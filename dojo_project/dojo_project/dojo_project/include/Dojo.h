#ifndef DOJO_H_
#define  DOJO_H_

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Time.h>

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
#define STATUS_LED			PORTD4
#define BPLAY				10				// Button Play/Resetpause Test
#define BPaus				11				// Button Pause (X) 
#define WTV_DOUT			12
#define WTV_CLK				13
#define WTV_RESET			9
#define WTV_PATH			0b00011000

#define OPCODE_PLAY_PAUSE	0xFFFE			//play track or stop the current, first send track number 0-511
#define OPCODE_STOP			0xFFFF			//stops the current playback
#define OPCODE_VOL			0xFFF0			//0xFFF0 is muted 0xFFF7 is max volume

enum State{SCAN, PLAY, GET_LIKES, LOAD_SD, LOAD_CONFIG};

extern unsigned int scanClosestBeacon();
extern void serialCom();
extern unsigned int scan();
extern void sendCommand(int com);
extern int readAnswer();
extern void filt(int answerByte);
extern void rssiCompare();
extern void firstResponse();
extern void compareBeacon();
extern void multiplexController(int multiplex_path);
extern void vibroController(boolean state);
extern unsigned int readValueDec(int array_length);
extern unsigned int castHexChararrToIntDec(char* array);
extern unsigned int testComparing(unsigned int test);
extern void sendWTVcommand(unsigned int command);
extern void resetWTV();
extern void pinSetupWTV();

#endif DOJO_H_
