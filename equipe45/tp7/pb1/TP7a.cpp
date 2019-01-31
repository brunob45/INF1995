/* Fichier : TP7a.cpp
 * Auteurs : Bruno Bousquet et Dominique Deschênes
 * Date	   : 25 février 2016
 * But     : Programme de contrôle d'une DEL avec le bouton sur le breadboard. 
 
-/!\- BRANCHEMENTS -/!\-
 * Alimentation Breadboard : Fils rouge et noir sur VCC et GND.
 * Connexion bouton : Fil bleu sur pin 3 du PORTD.
 * Connexion DEL : (+) -> pin 1 du PORTC, (-) -> pin 2 du PORTC.
 */

#include "../../tp.h"

// État du bouton.
volatile bool bouton = false;
// Compteur pour le nombre d'intermittences de la DEL.
volatile uint8_t compteur = 0;

// Que faire lorsque le bouton change d'état (interruption).
ISR(INT0_vect)
{
	// Lecture de l'état du bouton lors de l'interruption.
	int etatBouton = PIND & 0x04;
	// Attente pour le "debouncing".
	_delay_ms(20);

	// Si le bouton est encore dans le même état, on change son état.
	if (etatBouton == (PIND & 0x04))
		bouton = !bouton;
	
}

void initialisation()
{
	// On empêche les interruptions.
	cli();	

	DDRA = SORTIE; // On definit les ports A en sortie.
	DDRB = SORTIE; // On definit les ports B en sortie.
	DDRC = SORTIE; // On definit les ports C en sortie.
	DDRD = ENTREE; // On definit les ports D en entree.

        // Cette procédure ajuste le registre EIMSK
	// de ATmega324PA pour permettre les interruptions externes.
	EIMSK |= _BV(INT0);
	
	// Il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA.
	EICRA |= _BV(ISC00);

	// sei permet de recevoir a nouveau des interruptions.
	sei ();	
}

int main()
{
	initialisation();
	
	// On fait la boucle indéfiniment.
	for(;;)
	{
		bool ancienActif;		// État du bouton.
		
		// Si le bouton est actif.
		if(bouton)
		{
			_delay_ms(100);		// Attendre 100 ms.
			compteur++;		// Incrémenter le compteur.
			ancienActif = true;	// Indiquer que le bouton a été pressé.
		}
		
		// Si le bouton n'est plus actif, ou que le compteur est rendu à 120.
		if((!bouton && ancienActif) || compteur >= 120)
		{
			PORTA = VERT;		// Allumer la DEL est vert...
			_delay_ms(500);		// pendant 0.5 seconde.
			PORTA = ETEINT;		// Éteindre la DEL...
			_delay_ms(2000);	// pendant 2 secondes.

			// Faire allumer la DEL par intermittence (compteur/2) fois.
			for(int i = 0; i < compteur /2; i++)
			{
				PORTA = ROUGE;		// Allumer la DEL en rouge...
				_delay_ms(250);		// pendant 1/4 seconde.
				PORTA = ETEINT;		// Éteindre la DEL...
				_delay_ms(250);		// pendant 1/4 seconde.
			}

			PORTA = VERT;		// Allumer la DEL en vert...
			_delay_ms(1000);	// pendant 1 seconde.
			PORTA = ETEINT;		// Éteindre la DEL.

			// Réinitialiser les variables.
			compteur = 0;
			ancienActif = false;
		}			
	}
	
	return 0;
}
