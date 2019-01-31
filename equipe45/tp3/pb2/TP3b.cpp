// Fichier : TP3b.cpp
// Auteurs : Bruno Bousquet & Dominique Deschenes
// Date    : 21 janvier 2016
// But     : A chaque changement d'etat du bouton, la DEL change de couleur.

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

#include <tp.h>

int main()
{
	DDRA = SORTIE; //On definit les ports A en sortie
	DDRB = SORTIE; //On definit les ports B en sortie
	DDRC = SORTIE; //On definit les ports C en sortie
	DDRD = ENTREE; //On definit les ports D en entree
         
    enum etats {INIT, EA, EB, EC, ED, EE}; // liste les differents etats possibles
    uint8_t etatPresent = INIT; //identifie l'etat dans lequel on se trouve
    uint8_t couleur = 0; //compteur pour l'affichage de la couleur
    
	for(;;)  // boucle sans fin
	{
		switch(etatPresent)
		{
			case INIT : 				// dans le cas initial
				if(EstActive())			// si le bouton est pese
					etatPresent = EA;	// changer d'etat
				couleur = ROUGE;		// affecter la couleur rouge
				break;
			case EA : 					// dans le cas premier
				if(!EstActive())        // si le bouton n'est pas pese
					etatPresent = EB;   // changer d'etat
				couleur++;              // affecter la couleur ambree (changement rapide)
				break;
			case EB : 					// dans le cas second
				if(EstActive())         // si le bouton est pese
					etatPresent = EC;   // changer d'etat
				couleur = VERT;         // affecter la couleur vert
				break;
			case EC : 					// dans le cas troisieme
				if(!EstActive())        // si le bouton n'est pas pese
					etatPresent = ED;   // changer d'etat
				couleur = ROUGE;        // affecter la couleur rouge
				break;
			case ED : 					// dans le cas quatrieme
				if(EstActive())         // si le bouton est pese
					etatPresent = EE;   // changer d'etat
				couleur = ETEINT;       // affecter la couleur void
				break;
			case EE : 					// dans le cas cinq
				if(!EstActive())        // si le bouton n'est pas pese
					etatPresent = INIT; // changer d'etat
				couleur = VERT;         // affecter la couleur vert
				break;
		}
		
		PORTA = couleur;		// afficher la couleur choisie
	}

	return 0;
}
