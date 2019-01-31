// Fichier : TP3a.cpp
// Auteurs : Bruno Bousquet & Dominique Deschenes
// Date    : 21 janvier 2016
// But     : Apres 5 activation de l'interrupteur, allume la DEL libre pendant 1 seconde.

// Tableau des etats : 
//	Etat	Entree	Sortie	Prochain etat
//	INIT	0		0		INIT
//  INIT	1		0		E1
//	E1		0		0		E1
//	E1		1		0		E2
//	E2		0		0		E2
//	E2		1		0		E3
//	E3		0		0		E3
//	E3		1		0		E4
//	E4		0		0		E4
//	E4		1		0		E5
//	E5		X		1	    INIT

//Encodage des etats : 
// INIT = {000}
// EA   = {001}
// EB   = {010}
// EC   = {011}
// ED   = {100}
// EE   = {101}

#include <tp.h>

int main()
{
	DDRA = SORTIE; //On definit les ports A en sortie
	DDRB = SORTIE; //On definit les ports B en sortie
	DDRC = SORTIE; //On definit les ports C en sortie
	DDRD = ENTREE; //On definit les ports D en entree
         
    enum etats {INIT, EA, EB, EC, ED, EE};    // liste les differents etats possibles
    int etatPresent = INIT; //identifie l'etat dans lequel on se trouve
    
	for(;;)  // boucle sans fin
	{
		switch(etatPresent)
		{
			case INIT : 
				if(EstNouveauActive())
					etatPresent = EA;
				break;
			case EA : 
				if(EstNouveauActive())
					etatPresent = EB;
				break;
			case EB : 
				if(EstNouveauActive())
					etatPresent = EC;
				break;
			case EC : 
				if(EstNouveauActive())
					etatPresent = ED;
				break;
			case ED : 
				if(EstNouveauActive())
					etatPresent = EE;
				break;
			case EE : // apres avoir pese 5 fois sur le bouton, la lumiere fut
				PORTA = ROUGE;
				_delay_ms(DELAI);
				PORTA = ETEINT;
				etatPresent = INIT;
				break;
		}
	}

	return 0;
}
