// Fichier : TP5a.cpp
// Auteurs : Bruno Bousquet & Dominique Deschenes
// Date    : 8 fevrier 2016
// But     : A chaque changement d'etat du bouton, la DEL change de couleur. (I

// Tableau des etats
//	Etat	Entree	Sortie	Prochain etat
//	INIT	0		R		INIT
//  INIT	1		R		E1
//	E1		0		A		E2
//	E1		1		A		E1
//	E2		0		V		E2
//	E2		1		V		E3
//	E3		0		R		E4
//	E3		1		R		E3
//	E4		0		0		E4
//	E4		1		0		E5
//	E5		0		V		INIT
//	E5		1		V		E5

//Encodage des etats : 
// INIT = {000}
// EA   = {001}
// EB   = {010}
// EC   = {011}
// ED   = {100}
// EE   = {101}

#include <../../tp.h>

enum etats {INIT, EA, EB, EC, ED, EE};
volatile uint8_t etatPresent;

ISR(INT0_vect)
{
	int etatBouton = PIND & 0x04;
	_delay_ms(20);
	if (etatBouton == (PIND & 0x04))
	{
		++etatPresent;
		etatPresent %= 6;
	}
	
	EIFR |= (1 << INTF0) ;
}

void initialisation()
{
	cli();
	
	
	DDRA = SORTIE; //On definit les ports A en sortie
	DDRB = SORTIE; //On definit les ports B en sortie
	DDRC = SORTIE; //On definit les ports C en sortie
	DDRD = ENTREE; //On definit les ports D en entree

    // cette procédure ajuste le registre EIMSK
	// de ATmega324PA pour permettre les interruptions externes
	EIMSK |= _BV(INT0);
	
	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA
	EICRA |= _BV(ISC00);

	// sei permet de recevoir a nouveau des interruptions.
	sei ();
	
	
}

int main()
{  
	initialisation(); 
	uint8_t couleur = 0; //compteur pour l'affichage de la couleur
	for(;;)  // boucle sans fin
	{
		switch(etatPresent)
		{
			case INIT : 				// dans le cas initial
				couleur = ROUGE;		// affecter la couleur rouge
				break;
			case EA : 					// dans le cas premier
				couleur++;              // affecter la couleur ambree (changement rapide)
				break;
			case EB : 					// dans le cas second
				couleur = VERT;         // affecter la couleur vert
				break;
			case EC : 					// dans le cas troisieme
				couleur = ROUGE;        // affecter la couleur rouge
				break;
			case ED : 					// dans le cas quatrieme
				couleur = ETEINT;       // affecter la couleur void
				break;
			case EE : 					// dans le cas cinq
				couleur = VERT;         // affecter la couleur vert
				break;
		}
		
		PORTA = couleur;		// afficher la couleur choisie
	}

	return 0;
}
