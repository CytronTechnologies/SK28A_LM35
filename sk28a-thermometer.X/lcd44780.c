#include <htc.h>
#include "lcd44780.h"
#define _XTAL_FREQ 1000000

/*******************************************************************************
* PUBLIC FUNCTION: LCDCommand
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Send command byte to the 44780 LCD controller.
* Please refer to the datasheet for the list of commands.
*
*******************************************************************************/
void LCDCommand(unsigned char command) {

    RS = 0;
    __delay_us(5);
    // Move higher nibble first
    DB4 = (command & (1 << 4)) >> 4;
    DB5 = (command & (1 << 5)) >> 5;
    DB6 = (command & (1 << 6)) >> 6;
    DB7 = (command & (1 << 7)) >> 7;
    EN = 0; __delay_us(100); EN = 1; __delay_us(100);

    __delay_us(100);

    RS = 0;
    __delay_us(5);
    // Then move the lower nibble
    DB4 = (command & (1 << 0)) >> 0;
    DB5 = (command & (1 << 1)) >> 1;
    DB6 = (command & (1 << 2)) >> 2;
    DB7 = (command & (1 << 3)) >> 3;
    EN = 0; __delay_us(100); EN = 1; __delay_us(100);

    __delay_ms(5);
}

/*******************************************************************************
* PUBLIC FUNCTION: LCDInit
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize and clear the LCD display with blinking cursor.
* Note: This initialization routine is not from HD44780 datasheet. Instead, this one
* is from the Samsung S6A0069 controller.
*
*******************************************************************************/
void LCDInit() {
    // Initialization, according to the 44780 datasheet.
    // Modify the delay values accordingly.
    // Low delay values will cause LCD not to start or to produce garbled text.
    // The EN strobe must be more than 450ns.

    __delay_ms(750);

    LCDCommand(0b00100000);

    __delay_ms(1);

    LCDCommand(0b00100000);

    __delay_ms(1);

    LCDCommand(0b00100000);

    __delay_ms(1);

    LCDCommand(0b00101100);

    __delay_ms(1);

    LCDCommand(0b00001111);

    __delay_ms(1);

    LCDCommand(0b00000001);

    __delay_ms(5);

}

/*******************************************************************************
* PUBLIC FUNCTION: LCDWrite
*
* PARAMETERS:
* ~ character (unsigned byte)
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Send a character to the screen.
*
*******************************************************************************/
void LCDWrite(unsigned char inputChr) {

    RS = 1;
    __delay_us(5);
    DB4 = (inputChr & (1 << 4)) >> 4;
    DB5 = (inputChr & (1 << 5)) >> 5;
    DB6 = (inputChr & (1 << 6)) >> 6;
    DB7 = (inputChr & (1 << 7)) >> 7;
    EN = 0; __delay_us(100); EN = 1; __delay_us(100);

    __delay_us(100);

    RS = 1;
    __delay_us(5);
    DB4 = (inputChr & (1 << 0)) >> 0;
    DB5 = (inputChr & (1 << 1)) >> 1;
    DB6 = (inputChr & (1 << 2)) >> 2;
    DB7 = (inputChr & (1 << 3)) >> 3;
    EN = 0; __delay_us(100); EN = 1; __delay_us(100);

    __delay_ms(5); // Modify this accordingly. Too low will cause the system to display garbled text.

}

/*******************************************************************************
* PUBLIC FUNCTION: LCDWriteText
*
* PARAMETERS:
* ~ string, row and column
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Place a string in the specified row and column of the screen.
* +--+--+--+--+--+---------------------+
* |0 |1 |2 |3 |4 |5 ...etc             | <- row 0
* +--+--+--+--+--+---------------------+
* |0 |1 |2 |3 |4 |5 ...etc             | <- row 1
* +--+--+--+--+--+---------------------+
*
* This function can be used to place strings up to the 3rd row if used with the
* 20x4 HD44780 LCD.
* String printing method courtesy of Lucio di Jasio, from the book "Programming 32-bit Microcontrollers in C - Exploring the PIC32"
*
*******************************************************************************/
void LCDWriteText(const unsigned char* text,unsigned char row, unsigned char col) {


    unsigned char address_d = 0; // address of the data in the screen.
    switch (row) {
        case 0: address_d = 0x00 + col; // at zeroth row
            break;
        case 1: address_d = 0x40 + col; // at first row
            break;
        default: address_d = 0x00 + col; // returns to first row if invalid row number is detected
            break;
    }

    LCDCommand(0b10000000 | address_d);

    while (*text) // Place a string, letter by letter.
        LCDWrite(*text++);
}