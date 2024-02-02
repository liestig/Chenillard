/* 
 * File:   timer0.h
 * Author: Lies
 *
 * Created on 2 février 2024, 09:21
 */

// timer0.h

#ifndef TIMER0_H
#define TIMER0_H

#include <xc.h>

// Déclaration de la variable marche_arret
extern volatile unsigned char marche_arret;

// Prototypes des fonctions
void initTimer0(void);

#endif  // TIMER0_H
