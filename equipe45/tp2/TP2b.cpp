/*
 * Nom: compteur 32 bits
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Ceci est un exemple simple de programme 
 * Version: 1.1
 */

#include "../../tp.h"
#include "../../bouton.c"
#include "../../del.c"

int main()
{
	DDRA = SORTIE; // PORT A est en mode sortie
	DDRB = SORTIE; // PORT B est en mode sortie
	DDRC = SORTIE; // PORT C est en mode sortie
	DDRD = ENTREE; // PORT D est en mode entree

	while(1)  // boucle sans fin
	{
		if(EstActive())
		{
			couleur++;  // incremente le compteur
			PORTA = couleur;
		}
		else
			PORTA = ETEINT;
	}
	return 0; 
}

