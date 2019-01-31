// Fichier : TP4a.cpp
// Auteurs : Bruno Bousquet & Dominique Deschenes
// Date    : 21 janvier 2016
// But     : Attenuer la couleur d'une LED

#include <tp.h>

uint8_t vitesse = E0;
uint8_t vitesseMax = E100;

void delai_us(uint32_t delai)
{
	for(int i = 0; i < delai; i++)
		_delay_us(1);
}


void powerRoue(uint8_t& vitesse, const uint8_t& direction, const uint32_t& tempsms, const uint16_t& frequence)
{
    uint16_t iterations = frequence * tempsms / 1000;
	const uint16_t delai = 550; //60Hz -> 1800 400Hz -> 260 200Hz -> 550
    const uint16_t vitesseMax = E100;
	
	if(vitesse != 0)
	{
		PORTC = (ENABLE | direction) | (ENABLE | direction)<<2;
		delai_us(3);
	}
	
	while (iterations > 0)
	{
		for(uint32_t i = 0; i < vitesse; i++)
		{
			PORTC = (ENABLE | direction) | (ENABLE | direction)<<2;
			delai_us(delai);
		}
			
		for(uint32_t i = 0; i < vitesseMax - vitesse; i++)
		{
			PORTC = (!ENABLE & direction) | (!ENABLE & direction)<<2;
			delai_us(delai);
		}
		
		iterations--;
	}
}

int main()
{
	DDRA = SORTIE; //On definit les ports A en sortie
	DDRB = SORTIE; //On definit les ports B en sortie
	DDRC = SORTIE; //On definit les ports C en sortie
	DDRD = ENTREE; //On definit les ports D en entree
    
    bool direction = AVANCER;
    
	for(;;)  // boucle sans fin
	{
		/*if(vitesse != 0)
		{
			PORTC = (ENABLE | direction) | (ENABLE | direction)<<2;
			_delay_us(3);
		}
		
		for(uint32_t i = 0; i < vitesse; i++)
			PORTC = (ENABLE | direction) | (ENABLE | direction)<<2;

		for(uint32_t i = 0; i < vitesseMax - vitesse; i++)
			PORTC = (!ENABLE & direction) | (!ENABLE & direction)<<2;
			*/
		powerRoue(vitesse, direction, 2000, 200);
		
		vitesse = ++vitesse % 5;
		PORTC = vitesse << 4;
	}

	return 0;
}
