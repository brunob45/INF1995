#include "../../tp.h"
#include "usart.c"
#include "../memoire_24.cpp"

void initialisation()
{	
	DDRA = SORTIE; //On definit les ports A en sortie
	DDRB = SORTIE; //On definit les ports B en sortie
	DDRC = SORTIE; //On definit les ports C en sortie
	DDRD = SORTIE; //On definit les ports D en sortie
	initialisationUART () ;
}

void envoyer()
{
	uint8_t lettre;
	for(uint8_t j = 0; j < 0xff; j++)
	{
		Memoire24CXXX().lecture(j, &lettre);
			transmissionUART (lettre);
	}
}

int main()
{
	initialisation();
	PORTA = ROUGE;
	envoyer();
	PORTA = VERT;
	return 0;
}
