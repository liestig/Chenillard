/*
 * File:   main.c
 * Author: Li�s TIGUERCHA, Clement MABILE, Th�o MOUISSE
 *
 * Created on 5 f�vrier 2024, 08:53
 */
#include <xc.h>
#include "lcd.h"

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
#pragma config PBADEN = ON      // PORTB A/D Enable bit (PORTB<5:0> pins are configured as analog input on Reset)
#pragma config CCP3MX = PORTB5  // P3A/CCP3 Mux bit (P3A/CCP3 input/output is multiplexed with RB5)
#pragma config HFOFST = ON      // HFINTOSC Fast Start-up (HFINTOSC output and ready status are not delayed by the oscillator stable status)
#pragma config T3CMX = PORTC0   // Timer3 Clock input mux bit (T3CKI is on RC0)
#pragma config P2BMX = PORTD2   // ECCP2 B output mux bit (P2B is on RD2)
#pragma config MCLRE = EXTMCLR  // MCLR Pin Enable bit (MCLR pin enabled, RE3 input pin disabled)

// CONFIG4L
#pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
#pragma config LVP = ON         // Single-Supply ICSP Enable bit (Single-Supply ICSP disabled)
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

void initADC() {
    // Configurer l'entr�e RA0 en tant qu'entr�e analogique
    TRISAbits.TRISA0 = 1;  // RA0 en entr�e

    // Activer le mode "left-justified" (le r�sultat de la conversion est align� � gauche)
    ADCON2bits.ADFM = 1;

    // Configuration des tensions de r�f�rence VSS 0V et VDD 3V
    ADCON1bits.PVCFG0 = 0;
    ADCON1bits.NVCFG1  = 0;

    // Configure le r�sultat de la conversion pour une fr�quence de fonctionnement de FOSC/64
    ADCON2bits.ACQT = 1;  // Acquisition time = 2TAD
    ADCON2bits.ADCS = 2;  // Clock conversion bits = FOSC/64

    // Configure le registre ADCON0 pour s�lectionner l'entr�e analogique RA0/AN0
    // et active le convertisseur ADC
    ADCON0bits.CHS = 0;    // S�lectionne l'entr�e analogique RA0/AN0
    ADCON0bits.ADON = 1;   // Active le convertisseur ADC
}

void main(void) {
    // Initialisation
    
    // Configuration de l'ADC
    initADC();
    LCDInit(); 
    // Initialisation de l'�cran LCD
    float voltage; // Variable pour stocker la tension convertie
    
    // Boucle principale
    while(1) {
        ADCON0bits.GO_DONE = 1; // Lancer une nouvelle conversion
        // Attendre que la conversion soit termin�e
        while(ADCON0bits.GO_DONE); 
        
        // Lire le r�sultat de la conversion
      
        // Convertir la valeur ADC en tension (en volts)
        voltage = (float)ADRES / 1023 * 5.0; // Calculer la tension en volts
        
        
        // V�rifier si la valeur a chang� depuis la derni�re it�ration
            // Effacer l'�cran LCD
            LCDClear(); // Effacer l'�cran
            LCDGoto(0, 0); // Positionner le curseur en haut � gauche
            LCDWriteStr("Tension: "); // Afficher le texte
            LCDWriteFloat(9,0 , voltage); // Afficher la valeur ADC
            LCDGoto(0, 14); // Positionner le curseur en haut � gauche
            LCDWriteStr("V");
            __delay_ms(1000); // Attendre 1 seconde

            // Mettre � jour la valeur pr�c�dente
        
    }
}