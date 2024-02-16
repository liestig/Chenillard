#include <xc.h>
#include <stdio.h> // Pour sprintf
#include "lcd.h"
#include "uart.h"

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

#define _XTAL_FREQ 1000000UL // Définir la fréquence du microcontrôleur (1 MHz)

// Fonctions pour afficher un message sur l'écran LCD
void display_message(const char *message) {
    LCDClear(); // Efface l'écran LCD
    LCDWriteStr(message); // Affiche le message sur l'écran LCD
}

// Fonction pour initialiser l'EUSART
void init_UART() {
    // Configurer les broches TxD (RC6) et RxD (RC7) comme entrées/sorties
    TRISCbits.TRISC6 = 0; // TxD (sortie)
    TRISCbits.TRISC7 = 1; // RxD (entrée)

    // Configurer l'EUSART en mode asynchrone, 8 bits de données, 1 bit de stop
    TXSTA1 = 0b00100100; // BRGH=1 pour haute vitesse, TXEN=1 pour activer la transmission
    RCSTA1 = 0b10010000; // SPEN=1 pour activer le port série, CREN=1 pour activer la réception

    // Calculer la valeur de SPBRG en fonction de la fréquence d'horloge et du débit de transmission
    SPBRG1 = (_XTAL_FREQ / (16UL * 9600)) - 1;
}

// Fonction pour transmettre un caractère via l'EUSART
void UARTWriteChar(char data) {
    while (!TXSTAbits.TRMT); // Attendre que le tampon de transmission soit vide
    TXREG1 = data; // Écrire le caractère à transmettre dans le registre de transmission
}

// Fonction pour afficher un nombre entier sur l'écran LCD
void display_counter(int count) {
    char buffer[17];
    sprintf(buffer, "Compteur: %d", count);
    LCDGoto(0, 1); // Positionner le curseur à la ligne 2
    LCDWriteStr(buffer); // Afficher le compteur sur l'écran LCD
}

void main() {
    // Initialiser l'écran LCD
    LCDInit();
    
    // Initialiser l'EUSART
    init_UART();
    
    // Afficher un message d'initialisation sur l'écran LCD
    display_message("UART 9600 bauds");

    // Déclarer une variable pour le compteur
    int counter = 0;
    
    // Boucle infinie
    while (1) {
        // Afficher le compteur sur l'écran LCD
        display_counter(counter);
        
        // Incrémenter le compteur
        counter++;
        
        // Temporisation (facultatif)
        __delay_ms(500);
        
        // Transmettre le compteur via l'EUSART
        UARTWriteChar((char)(counter + '0')); // Convertir le compteur en caractère et l'envoyer
    }
}