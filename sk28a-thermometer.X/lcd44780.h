// LCD44780-compatible S6A0069 LCD module.
#define RS LATBbits.LATB2
#define EN LATBbits.LATB3
#define DB4 LATBbits.LATB4
#define DB5 LATBbits.LATB5
#define DB6 LATBbits.LATB6
#define DB7 LATBbits.LATB7
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
void LCDCommand(unsigned char);

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
void LCDInit();

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
void LCDWrite(unsigned char);

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
void LCDWriteText(const unsigned char*, unsigned char, unsigned char);