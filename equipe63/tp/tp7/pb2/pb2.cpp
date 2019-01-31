/*
 INF1995: TP7 probleme2
 Auteurs  JULIO H. DANDJINOU et J. Staelle MASSADO FOKO (groupe 63)
 Date: 28/ 02/ 2016
 Description: Ce programme permet d' effectuer des lectures sur une photoresistance (selon l'intensite limuneuse).
             
             
             BRANCHEMENT des cables:
       Pour l'allumage de LED, on utilise le PC1 (PORTC 1)
     
       Pour le cable de donnees, nous utilisons PORTA (PORTA 0)
*/






#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define  F_CPU 8000000UL
#include <util/delay.h>
#include "can.h"


 
can convertisseur;  //variable convertisseur
 

    void initialisation ( void ) {
        // cli est une routine qui bloque toutes les interruptions.
        // Il serait bien mauvais d'etre interrompu alors que
        // le microcontroleur n'est pas pret.
        cli ();
        // configurer et choisir les ports pour les entrees
        // et les sorties. DDRx... Initialisez bien vos variables
        DDRC = 0xff;  
        DDRA = 0x00;  
        
        ADCSRA |= _BV(ADSC); //Demarrer  le convertisseur 
        // sei permet de recevoir a nouveau des interruptions.
        sei ();
    }





    int main(void)
    {   
        initialisation();  // appel de la fonction d'initialisation.
        uint16_t k ;   
        uint8_t valeur1 = 150;
        uint8_t valeur2 = 220;
        
        //A chaque cycle , on lit la nouvelle valeur du convertisseur 
        //et en fonction de la valeur lue, nous attribuons la couleur souhaitee.
         for(;;)
        {
            k = (convertisseur.lecture(0));
            
            k >>=2;
            
            if(k < valeur1)
            {
                PORTC = 0x01; // La DEL prend la couleur verte quand la lumiere est basse.
            }
            
            else if(k < valeur2)
            { // La DEL prend la couleur ambree quand la lumiere a un bon niveau.
                PORTC = 0x02;
                _delay_ms(10);
                PORTC = 0x01;
                _delay_ms(10);
                                        
            }
            
            else 
            {
                PORTC = 0x02;    // La DEL prend la couleur rouge quand la lumiere est trop forte.
            }       
        } 
        return 0;
    }