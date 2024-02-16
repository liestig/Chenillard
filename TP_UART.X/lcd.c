
/*
 * File:   lcd.c
 * Author: lal
 *
 * Created on 16 mars 2015, 09:53
 */

/*
 * Updated by JMC on 29th april 2015
 * - LCDInit() made compliant with generic LCD screens
 * - LCDPutByte(UINT8_T value) displays 3 digits instead of 2
 */

//*****************************************************************************
// Include and Header files
//*****************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "general.h"
#include "lcd.h"


UINT8_T ReadByte(void)
{
    UINT8_T status;

    LCD_D0_DIR = INP;
    LCD_D1_DIR = INP;
    LCD_D2_DIR = INP;
    LCD_D3_DIR = INP;

    LCD_EN = ENABLE;
    Nop();
    Nop();

    status = PORTD << 4;
    LCD_EN = DISABLE;

    LCD_EN = ENABLE;
    Nop();
    Nop();

    status |= PORTD & 0x0F;
    LCD_EN = DISABLE;

    LCD_D0_DIR  = OUTP;
    LCD_D1_DIR  = OUTP;
    LCD_D2_DIR  = OUTP;
    LCD_D3_DIR  = OUTP;

    return(status);
}

void WaitLCD(void)
{
    UINT8_T status;

    LCD_RS = CLEAR;
    LCD_RW = READ;

    do {
        status = ReadByte();
    } while (status & 0x80);

    LCD_RW = WRITE;
}

void WriteNibble(UINT8_T cmd, UINT8_T data)
{
    LCD_RS = (cmd ? CLEAR : SET);
    LCD_RW = WRITE;
    LCD_EN = ENABLE;

    LCD_DAT0 = LCD_DAT1 = LCD_DAT2 = LCD_DAT3 = CLEAR;
    Nop();
    Nop();

    LATD = (LATD & 0xF0) | (data & 0x0F);
    Nop();
    Nop();

    LCD_EN = DISABLE;
}

void WriteByte(UINT8_T cmd, UINT8_T data)
{
    WriteNibble(cmd, data >> 4);
    WriteNibble(cmd, data);
}

void LCDInit(void)
{
    LCD_PWR_DIR = OUTP;             // Power line as output
    LCD_EN_DIR  = OUTP;             // LCD enable as output
    LCD_RW_DIR  = OUTP;             // LCD R/W as output
    LCD_RS_DIR  = OUTP;             // LCD register select as output
    LCD_D0_DIR  = OUTP;             // LCD data 0 as output
    LCD_D1_DIR  = OUTP;             // LCD data 1 as output
    LCD_D2_DIR  = OUTP;             // LCD data 2 as output
    LCD_D3_DIR  = OUTP;             // LCD data 3 as output

    LCD_ON();                       // Turn the LCD display on
    _delay(30000);                  // Wait at least 40 ms (here 30000 x 4 �s = 120 ms)
    WriteNibble(TRUE, 0x03);        // Function set (interface is 8 bits long) - 1st write
    _delay(3000);                   // Wait at least 4.1 ms (here 3000 x 4 �s = 12 ms)
    WriteNibble(TRUE, 0x03);        // Function set (interface is 8 bits long) - 2nd write
    _delay(100);                    // Wait at least 100 us (here 100 x 4 �s = 400 us)
    WriteNibble(TRUE, 0x03);        // Function set (interface is 8 bits long) - 3rd write
    _delay(100);
    WriteNibble(TRUE, 0b00000010);  // Function set (set interface to 4 bits long)
    WaitLCD();                      // check busy flag
    WriteNibble(TRUE, 0b00000010);  // Function set (interface is now 4 bits long):
    WriteNibble(TRUE, 0b00001000);  // 2 display lines (N = 1), 5x8 font (F = 0)
    WaitLCD();
    WriteNibble(TRUE, 0b00000000);  // Display off
    WriteNibble(TRUE, 0b00001000);  // (Display off, Cursor off, Blinking off)
    WaitLCD();
    WriteNibble(TRUE, 0b00000000);  // Display on
    WriteNibble(TRUE, 0b00001100);  // (Display on, Cursor off, Blinking off)
// ** Comment previous line and uncomment next line to activate Blinking Cursor
//    WriteNibble(TRUE, 0b00001111);  // (Display on, Cursor on, Blinking on)
    WaitLCD();
    WriteNibble(TRUE, 0b00000000);  // Clear display
    WriteNibble(TRUE, 0b00000001);  //
    WaitLCD();
    WriteNibble(TRUE, 0b00000000);  // Entry mode set
    WriteNibble(TRUE, 0b00000110);  // Increment (I/D = 1), No shift (S = 0)
    WaitLCD();
}

void LCDClear(void)
{
     WriteByte(TRUE, 0x01);
     _delay(12500);
     WaitLCD();
}

void LCDGoto(UINT8_T column, UINT8_T row)
{
    if ((row > (NB_LINES - 1)) || (column > (NB_COL - 1)))
        return;

    WriteByte(TRUE, (row == 1) ? (0xC0 | column) : (0x80 | column));
    WaitLCD();
}

void LCDPutChar(UINT8_T data)
{
    WriteByte(FALSE, data);
    WaitLCD();
}

void LCDPutByte(UINT8_T value)
{
    LCDPutChar(value / 100 + '0');          // display hundreds digit
    LCDPutChar((value % 100) / 10 + '0');   // display tenths digit
    LCDPutChar(value % 10 + '0');           // display units digit
}

void LCDWriteStr(const char *string)
{
  UINT8_T i = 0;

  while (string[i])
    LCDPutChar(string[i++]);
}

void LCDWriteHexa(UINT8_T column, UINT8_T row, UINT8_T value){
    char *hexa = "0123456789ABCDEF";
    char string [3];

    string[0] = hexa[value >> 4];
    string[1] = hexa[value & 0x0F];
    string[2] = '\0';

    LCDGoto(column, row);
    LCDWriteStr(string);
}

void LCDWriteFloat(UINT8_T column, UINT8_T row, float value) {
    UINT16_T integer;
    char string[5];

    integer = (UINT16_T)(value * 100) % 1000;
    string[0] = integer / 100 + '0';
    string[1] = '.';
    string[2] = (integer / 10) % 10 + '0';
    string[3] = integer % 10 + '0';
    string[4] = '\0';

    LCDGoto(column, row);
    LCDWriteStr(string);
}


void LCDWriteInt(UINT8_T column, UINT8_T row, UINT8_T value) {
    char string[3];

    string[0] = value / 10 + '0';
    string[1] = value % 10 + '0';
    string[2] = '\0';

    LCDGoto(column, row);
    LCDWriteStr(string);
}

void LCDShiftDisplay(INT8_T shiftCount) {

    while (shiftCount-- > 0) {
        WriteByte(TRUE, 0b00011000); // shift (S/C=1), to the left (R/L=0)
    }
}



