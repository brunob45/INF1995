#include "tp.h"

int main()
{
	DDRB = SORTIE;
	if (EstActive())
		PORTB = ROUGE;
	
	return 0;
}
