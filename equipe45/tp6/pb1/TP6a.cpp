/* Fichier : TP6a.cpp
 * Auteurs : Bruno Bousquet & Dominique Deschênes
 * Date    : 25 février 2016
 * But     : Écrit une chaine de caractères en mémoire, et la relis pour confirmer l'écriture.

 *-/!\- Branchements -/!\_
 *  Cavalier sur MémEn
 *  DEL : (+) -> PORTA(1), (-) -> PORTA(2) 
 * */

#include "../../tp.h"
#include "../memoire_24.cpp"

void initialisation()
{
	DDRA = SORTIE;
	DDRB = SORTIE;
	DDRC = SORTIE;
	DDRD = SORTIE;

	Memoire24CXXX();
	PORTA=ETEINT;
}

int main()
{
	initialisation();
	uint8_t chaine[20] = "ECOLE POLYTECHNIQUE";
	chaine[20] = 0x00;
	uint8_t chaineLue[20];
	bool estPareil = true;
	
	// Lecture.
	for(uint16_t i = 0; i < 20; i++)
	{
		Memoire24CXXX().ecriture(i, chaine[i]);
	}

	// Écriture.
	for(uint16_t i = 0; i < 20; i++)
	{
		Memoire24CXXX().lecture(i, &chaineLue[i]);
	}	

	// Comparaison.
	for(uint16_t i = 0; i < 20 && estPareil; i++)
	{
		estPareil = chaine[i] == chaineLue[i];
	}

	PORTA = estPareil ? VERT : ROUGE ; 
	_delay_ms(500);
	PORTA = ETEINT;

	return 0;
}
