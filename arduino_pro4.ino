#include <Arduino.h>
#include <EEPROM.h>
#include "../../../../../opt/arduino-1.8.5/hardware/arduino/avr/cores/arduino/USBAPI.h"
#include "../../../../../opt/arduino-1.8.5/hardware/arduino/avr/cores/arduino/HardwareSerial.h"
#include "../../../../../opt/arduino-1.8.5/hardware/tools/avr/avr/include/stdlib.h"

#define RESET 9
#define NEXT 8
#define PREV 7
#define PLAY 6
#define BUSY 5
#define DOUT 4
#define DCLK 3
#define OPCODE_PLAY_PAUSE 0xFFFE
#define OPCODE_STOP 0xFFFF
#define OPCODE_STOP 0xFFFF
#define OPCODE_VOL 0xFFF0

void resetModule();
void pinSetup();
void directAudioControl();

void inventoryManager();

unsigned file = 0;
unsigned keypress = 20;

void setup() {
    Serial.begin(115200);
    pinMode(13, OUTPUT);
    pinSetup();
    resetModule();
    for (int i = 0; i < EEPROM.length(); i++) {
        EEPROM.write(i, i + 2);
    }
}

void loop() {
    if (Serial.available() > 0) {
        inventoryManager();
    } else {

    }
    //directAudioControl();
}

void inventoryManager() {
    char opcode = ' ';
    char address[4] = {' ', ' ', ' ', '\0'};
    char value[4] = {' ', ' ', ' ', '\0'};
    char temp = ' ';
    unsigned counter = 0;
    delay(1);
    while (Serial.available() > 0) {
        temp = Serial.read();
        if (temp != ';') {  //';' terminates instruction
            if (counter == 0) {
                opcode = temp;
            } else if (counter >= 1 && counter <= 3) {
                address[counter - 1] = temp;
            } else if (counter >= 4 && counter <= 6) {
                value[counter - 4] = temp;
            }
            counter ++;
        } else {
            int a = atoi(address);
            int v = atoi(value);

            /*
            Serial.print("opcode: ");
            Serial.print(opcode);
            Serial.print(" address: ");
            Serial.print(a);
            Serial.print(" value: ");
            Serial.print(v);
            Serial.print("\n");
            */

            if (opcode == 'q') {
                Serial.println(EEPROM.read(a));
            } else if (opcode == 'p') {
                EEPROM.write(a, v);
            }
            counter = 0;
            break;
        }
    }
}


void directAudioControl() {
    while (true) {
        if (Serial.available() > 0) {
            char command = Serial.read();
            Serial.println(command);
            switch (command) {
                case 'r':
                    resetModule();
                    break;
                case 'p':
                    sendCommand((unsigned int) file);
                    sendCommand(OPCODE_PLAY_PAUSE);
                    break;
                case 's':
                    sendCommand(OPCODE_STOP);
                    break;
                case 'n':
                    if (file++ == 11) file = 0;
                    Serial.print(file);
                    digitalWrite(NEXT, LOW);
                    delay(keypress);
                    digitalWrite(NEXT, HIGH);
                    break;
                case 'b':
                    if (file-- == 255) file = 10;
                    digitalWrite(PREV, LOW);
                    delay(keypress);
                    digitalWrite(PREV, HIGH);
                    break;
                case '+':
                    if (file++ == 11) file = 0;
                    digitalWrite(NEXT, LOW);
                    delay(keypress);
                    digitalWrite(NEXT, HIGH);
                    while (digitalRead(BUSY) == HIGH);
                    digitalWrite(PLAY, LOW);
                    delay(keypress);
                    digitalWrite(PLAY, HIGH);
                    break;
                case '-':
                    if (file-- == 255) file = 10;
                    digitalWrite(PREV, LOW);
                    delay(keypress);
                    digitalWrite(PREV, HIGH);
                    while (digitalRead(BUSY) == HIGH);
                    digitalWrite(PLAY, LOW);
                    delay(keypress);
                    digitalWrite(PLAY, HIGH);
                    break;
                case 't':
                    digitalWrite(PLAY, LOW);
                    delay(keypress);
                    digitalWrite(PLAY, HIGH);
                    break;
                case 'x':
                    sendCommand(OPCODE_PLAY_PAUSE);
                    break;
            }
        }
    }
}

void pinSetup() {
    pinMode(RESET, OUTPUT);
    digitalWrite(RESET, LOW);
    pinMode(NEXT, OUTPUT);
    digitalWrite(NEXT, HIGH);
    pinMode(PREV, OUTPUT);
    digitalWrite(PREV, HIGH);
    pinMode(PLAY, OUTPUT);
    digitalWrite(PLAY, HIGH);

    pinMode(DOUT, OUTPUT);
    digitalWrite(DOUT, HIGH);
    pinMode(DCLK, OUTPUT);
    digitalWrite(DOUT, HIGH);

    pinMode(BUSY, INPUT);
}

void resetModule() {
    digitalWrite(RESET, LOW);
    delay(100);
    digitalWrite(RESET, HIGH);
    delay(500);
}

void sendCommand(unsigned int command) {
    digitalWrite(DCLK, LOW);
    delayMicroseconds(1900);
    for (byte i = 0; i < 16; i++) {
        delayMicroseconds(100);
        digitalWrite(DCLK, LOW);
        digitalWrite(DOUT, LOW);
        if ((command & 0x8000) != 0) {
            digitalWrite(DOUT, HIGH);
        }
        delayMicroseconds(100);
        digitalWrite(DCLK, HIGH);
        command = command << 1;
    }
}

void writeInventory(unsigned index, char *btId) {

}

void requestManifest() {

}

int getClosestBeacon() {
    return 0;
}