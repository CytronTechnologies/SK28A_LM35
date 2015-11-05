/*********************************************************************************************
* SK28A CELCIUS THERMOMETER WITH LM35 AND FIXED VOLTAGE REFERENCE PROJECT                    *
* BY NG YONG HAN                                                                             *
**********************************************************************************************/

#include <htc.h>
#include "lcd44780.h"
#define _XTAL_FREQ 1000000

__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_ON & CP_OFF & CPD_OFF & BOREN_OFF & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & VCAPEN_OFF & PLLEN_OFF & STVREN_OFF & BORV_LO & LVP_OFF);

/*******************************************************************************
* PUBLIC FUNCTION: init()
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the peripherals in SK28A. 1MHz Internal Oscillator is used.
*
*******************************************************************************/
void init()
{
    OSCCONbits.IRCF3 = 1;   // 1MHz internal oscillator
    OSCCONbits.IRCF2 = 0;   //
    OSCCONbits.IRCF1 = 1;   //
    OSCCONbits.IRCF0 = 1;   //
    OSCCONbits.SCS0 = 0;    //
    OSCCONbits.SCS1 = 1;    //

    TRISA = 0x00;           // TRISA are all outputs at start
    TRISB = 0x00;           // TRISB are all outputs at start
    PORTA = 0x00;           // PORTA cleared
    PORTB = 0x00;           // PORTB cleared
    CM1CON0 = 0x00;         // Shut off comparators
    CM2CON0 = 0x00;         //
    LCDCONbits.LCDEN = 0;   // Shut off LCD module
    SRCON0bits.SRLEN = 0;   // Shut off S-R Latch module
    CPSCON0bits.CPSON = 0;  // Shut off Capacitive sensing module
    APFCONbits.SSSEL = 1;   // /SS input at RA0
    LCDSE0 = 0x00;          // All LCD Segments disabled
    LCDSE1 = 0x00;          //
}

/*******************************************************************************
* PUBLIC FUNCTION: init_adc()
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Initialize the ADC in SK28A. RA5 is the default selected ADC channel.
* Modify this according to the project if neccessary.
*******************************************************************************/
void init_adc()
{
    TRISAbits.TRISA5 = 1;   // Input at ADC port
    ANSELA = 0b00010000;    // RA5 (AN4) selected as ADC channel
    ANSELB = 0x00;          // All PORTB are digital
    FVRCON = 0b11000001;    // Fixed Voltage Reference enabled, FVR ready, Temp Sensor disabled,
                            // Temp Sensor Low Range, Comparator FVR turned off, Fixed Ref at 1.024V
    ADCON0 = 0b00010000;    // RA5 (AN4), Not Done, ADC off
    ADCON1 = 0b10000011;    // Right justified, Fosc/2, Vref using FVR
    ADCON0bits.ADON = 1;    // Switch on ADC module
}

/*******************************************************************************
* PUBLIC FUNCTION: main()
*
* PARAMETERS:
* ~ void
*
* RETURN:
* ~ void
*
* DESCRIPTIONS:
* Main body of the Thermometer program:
* 1.) Initialize SK28A
* 2.) Initialize ADC module and HD44780 module
* 3.) Send LCD command - no blinking cursor
* 4.) Write "Tempr:" on the first line of the LCD
* 5.) Start A/D conversion.
* 6.) If conversion complete, copy A/D result (10-bit ADRES) into variable ADCresult and divide by 10
* 7.) Get the tenth of the result into variable ADCresultDec
* 8.) Convert the values into readable ASCII format (string form)
* 9.) Put the string into the LCD
* 10.) Delay 1 second and go back to 5.)
*
*******************************************************************************/
void main()
{
    unsigned int ADCresult;
    unsigned char ADCresultDec;
    unsigned char tempText[9];
    init();                                                 // (1)
    __delay_ms(5);
    init_adc();                                             // (2)
    __delay_ms(5);                                          //
    LCDInit();                                              // (2)
    LCDCommand(0b00001100);                                 // (3)
    LCDWriteText("Tempr:",0, 0);                            // (4)
    while(1)
    {
        ADCON0bits.GO = 1;                                  // (5)
        while(ADCON0bits.GO);                               //

        ADCresult = ADRES / 10;                             // (6)
        ADCresultDec = ADRES % 10;                          // (7)

        tempText[0] = (ADCresult / 1000) + 0x30;            // (8)
        tempText[1] = ((ADCresult / 100) % 10)  + 0x30;     //
        tempText[2] = ((ADCresult / 10)  % 10) + 0x30;      //
        tempText[3] = ADCresult % 10 + 0x30;                //
        tempText[4] = '.';                                  //
        tempText[5] = ADCresultDec + 0x30;                  //
        tempText[6] = 223;                                  // (223 is the degree symbol. Refer the HD44780 datasheet!)
        tempText[7] = 'C';                                  //
        tempText[8] = '\0';                                 //

        if (tempText[0] == '0')                             // Cover up leading zeroes.
            tempText[0] = ' ';                              //
        if (tempText[1] == '0')                             //
            tempText[1] = ' ';                              //

        LCDWriteText(tempText, 1, 0);                       // (9)

        __delay_ms(1000);                                   // (10)
    }
}