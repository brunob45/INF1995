// Fichier : TP4a.cpp
// Auteurs : Bruno Bousquet & Dominique Deschenes
// Date    : 21 janvier 2016
// But     : Attenuer la couleur d'une LED

#include <../../tp.h>

///<summary>
///affiche une jolie transition entre deux couleurs
///</summary>
///<param name ='origine'>couleur d'origine de la DEL</param>
///<param name ='cible'>couleur cible de la DEL</param>
void fadeDEL(uint8_t origine, uint8_t cible)
{
    uint16_t a = 180;
    uint16_t b = a;
	
	while (a > 0)
	{
		for(uint32_t i = 0; i < a; i++)
		{
			PORTA = origine;
			_delay_us(90);
		}
			
		for(uint32_t i = 0; i < b - a; i++)
		{
			PORTA = cible;
			_delay_us(90);
		}
			
		a -= 1;
	}
}

int main()
{
	DDRA = SORTIE; //On definit les ports A en sortie
	DDRB = SORTIE; //On definit les ports B en sortie
	DDRC = SORTIE; //On definit les ports C en sortie
	DDRD = ENTREE; //On definit les ports D en entree
    
	for(;;)  // boucle sans fin
	{

		fadeDEL(VERT, ETEINT);
		
		fadeDEL(ETEINT, VERT);
	}

	return 0;
}
