#include "timer0.h"
#include <xc.h>

void init(unsigned char param1, unsigned char param2, unsigned char param3) {
    T0CON = param1; // Configuration du timer0
    TMR0H = param2; // Initialisation du temps de delai ( Bits de poids fort )
    TMR0L = param3; //idem ( Bits de poids faible )
}

void delai() {
    while (1) //On entre dans une boucle infinie
    {
        if (TMR0IF == 1) // Si le TMR0 a fini de compter ( Flag TMR0IF est leve )
        {
            TMR0IF = 0; //On a baisse le flag pour redemarrer le timer
            return; //On quitte la boucle en retournant au programme principal
        }
    }
}
