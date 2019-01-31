#include "../../tp.h"
#include "usart.c"

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
	char mots[21] = "Le robot en INF1995\n";

	uint8_t i, j;
	for ( i = 0; i < 100; i++ ) 
		for ( j=0; j < 20; j++ ) 
			transmissionUART ( mots[j] );
}

int main()
{
	initialisation();
	PORTA = ROUGE;
	envoyer();
	PORTA = VERT;
	return 0;
}
