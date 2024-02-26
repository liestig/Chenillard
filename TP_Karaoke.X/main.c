/*
 * File:   main.c
 * Author: Liès TIGUERCHA, Clement MABILE, Théo MOUISSE
 *
 * Created on 5 février 2024, 08:53
 */

#include <xc.h>
#include "lcd.h"
#include "leds.h"
#include "timer0.h"


// CONFIG1H
#pragma config FOSC = INTIO7    // Oscillator Selection bits (Internal oscillator block, CLKOUT function on OSC2)
#pragma config PLLCFG = OFF     // 4X PLL Enable (Oscillator used directly)
#pragma config PRICLKEN = ON    // Primary clock enable bit (Primary clock is always enabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
#pragma config IESO = OFF       // Internal/External Oscillator Switchover bit (Oscillator Switchover mode disabled)

// CONFIG2L
#pragma config PWRTEN = OFF     // Power-up Timer Enable bit (Power up timer disabled)
#pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled in hardware only (SBOREN is disabled))
#pragma config BORV = 190       // Brown Out Reset Voltage bits (VBOR set to 1.90 V nominal)

// CONFIG2H
#pragma config WDTEN = OFF      // Watchdog Timer Enable bits (Watch dog timer is always disabled. SWDTEN has no effect.)
#pragma config WDTPS = 32768    // Watchdog Timer Postscale Select bits (1:32768)

// CONFIG3H
#pragma config CCP2MX = PORTC1  // CCP2 MUX bit (CCP2 input/output is multiplexed with RC1)
#pragma config PBADEN = OFF     // PORTB A/D Enable bit (PORTB<5:0> pins are configured as digital I/O on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON        // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
#pragma config XINST = OFF      // Extended Instruction Set Enable bit (Instruction set extension and Indexed Addressing mode disabled (Legacy mode))

// CONFIG5L
#pragma config CP0 = OFF        // Code Protection Block 0 (Block 0 (000800-003FFFh) not code-protected)
#pragma config CP1 = OFF        // Code Protection Block 1 (Block 1 (004000-007FFFh) not code-protected)
#pragma config CP2 = OFF        // Code Protection Block 2 (Block 2 (008000-00BFFFh) not code-protected)
#pragma config CP3 = OFF        // Code Protection Block 3 (Block 3 (00C000-00FFFFh) not code-protected)

// CONFIG5H
#pragma config CPB = OFF        // Boot Block Code Protection bit (Boot block (000000-0007FFh) not code-protected)
#pragma config CPD = OFF        // Data EEPROM Code Protection bit (Data EEPROM not code-protected)

// CONFIG6L
#pragma config WRT0 = OFF       // Write Protection Block 0 (Block 0 (000800-003FFFh) not write-protected)
#pragma config WRT1 = OFF       // Write Protection Block 1 (Block 1 (004000-007FFFh) not write-protected)
#pragma config WRT2 = OFF       // Write Protection Block 2 (Block 2 (008000-00BFFFh) not write-protected)
#pragma config WRT3 = OFF       // Write Protection Block 3 (Block 3 (00C000-00FFFFh) not write-protected)

// CONFIG6H
#pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers (300000-3000FFh) not write-protected)
#pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block (000000-0007FFh) not write-protected)
#pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not write-protected)

// CONFIG7L
#pragma config EBTR0 = OFF      // Table Read Protection Block 0 (Block 0 (000800-003FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR1 = OFF      // Table Read Protection Block 1 (Block 1 (004000-007FFFh) not protected from table reads executed in other blocks)
#pragma config EBTR2 = OFF      // Table Read Protection Block 2 (Block 2 (008000-00BFFFh) not protected from table reads executed in other blocks)
#pragma config EBTR3 = OFF      // Table Read Protection Block 3 (Block 3 (00C000-00FFFFh) not protected from table reads executed in other blocks)

// CONFIG7H
#pragma config EBTRB = OFF      // Boot Block Table Read Protection bit (Boot Block (000000-0007FFh) not protected from table reads executed in other blocks)

#define DO 119
#define FA 89
#define MI 94
#define RE 106
#define SOL 79
#define LA 70

void initLEDs(){
    LED0_DIR = 0;
    LED1_DIR = 0;
    LED2_DIR = 0;
    LED3_DIR = 0;
    LED0_STATE = 0;
    LED1_STATE = 0;
    LED2_STATE = 0;
    LED3_STATE = 0;
}

void turnOffLEDs() {
    LED0_STATE = 0;
    LED1_STATE = 0;
    LED2_STATE = 0;
    LED3_STATE = 0;
}

void turnOnLED(int ledNumber) {
    turnOffLEDs();
    switch (ledNumber) {
        case 0:
            LED0_STATE = 1;
            break;
        case 1:
            LED1_STATE = 1;
            break;
        case 2:
            LED2_STATE = 1;
            break;
        case 3:
            LED3_STATE = 1;
        default:
            // Invalid LED number
            break;
    }
}

void main() {
    // INIT paroles, notes, tempo et décalage
    const int notes[44] = {DO, DO, SOL, SOL, LA, LA, SOL, FA, FA, MI, MI, RE, RE, DO, SOL, SOL, FA, FA, MI, MI, MI, RE, SOL, SOL, FA, FA, MI, MI, MI, RE, DO, DO, SOL, SOL, LA, LA, SOL, FA, FA, MI, MI, RE, RE, DO};
    const char *paroles[44] = {"Ah! ", "vous ", "di", "rai ", "je ", "ma", "man, ", "Ce ", "qui ", "cau", "se ", "mon ", "tour", "ment! ", "Pa", "pa ", "veut ", "que ", "je ", "rai", "son", "ne ", "comme ", "u", "ne ", "gran", "de ", "per", "son", "ne; ", "Moi, ", "je ", "dis ", "que ", "les ", "bon", "bons ", "va", "lent ", "mieux ", "que ", "la ", "rai", "son."};
    const int nbCharByTempo[44] = {4, 5, 2, 4, 3, 2, 5, 3, 4, 3, 3, 4, 4, 6, 2, 3, 5, 4, 3, 3, 3, 3, 6, 1, 3, 4, 3, 3, 3, 4, 5, 3, 4, 4, 3, 5, 2, 5, 6, 4, 3, 3, 3};
    const int tempo[44] = {1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 0};
    
    TRISCbits.TRISC2 = 0;
    CCP1CON = 0x0C;
    T2CON = 0x05;
    
    int PAUSE = 0x00;
    
    LCDInit();
    LCD_ON();
    
    int ledNumberToSwitchOn = 0;

    initLEDs();
    LCDShiftDisplay(28);
    
    int decalage = 0;
    for (int i = 0; i < 44; i++) {
        LCDWriteStr(paroles[i]);
        
        PR2 = notes[i];
        CCPR1L = notes[i] / 2;
        
        LCDShiftDisplay(nbCharByTempo[i]);
        
        if (tempo[i] == 1) {
            init(0x82, 0xDC, 0x0B);
        } else {
            init(0x83, 0xDC, 0x0B);
        }
        
        delai();
        PR2 = PAUSE;
        init(0x80, 0xB0, 0x3C);
        delai();
        
        decalage += nbCharByTempo[i];        
        if(decalage >= 18){
            LCDClear();
            LCDShiftDisplay(28);
            decalage = 0;
        }
        
        ledNumberToSwitchOn++;
        turnOnLED(ledNumberToSwitchOn%4);
    }
    return;
}
