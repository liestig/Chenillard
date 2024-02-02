#include <xc.h>
#include "timer0.h"

// Configuration du Timer0
void initTimer0() {
    T0CON = 0b10000011; // 16-bit, pas de pr�diviseur, front montant, source interne
    TMR0L = 49911 & 0xFF;   // Les 8 bits les moins significatifs
    TMR0H = (49911 >> 8) & 0xFF;  // Les 8 bits les plus significatifs
    INTCONbits.TMR0IF = 0; // R�initialisation du flag d'overflow
    INTCONbits.TMR0IE = marche_arret;  // Activer l'interruption Timer 0
}

