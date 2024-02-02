#include <xc.h>
#include "button.h"

void initButton() {
    TRISBbits.TRISB0 = 1; // Configure RB0/INT0 as input
    INTCONbits.INT0IE = 1; // Enable INT0 interrupt
    INTCON2bits.INTEDG0 = 0; // Trigger INT0 on falling edge
    INTCONbits.INT0IF = 0; // Clear INT0 flag
    INTCONbits.GIE = 1; // Enable global interrupts
}
