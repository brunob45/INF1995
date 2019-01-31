#define AVANCER 0x00
#define RECULER 0x01
#define ENABLE  0x02

enum vitesses {E0, E25, E50, E75, E100};

///<summary>
///affiche une jolie transition entre deux couleurs
///</summary>
///<param name ='origine'>couleur d'origine de la DEL</param>
///<param name ='cible'>couleur cible de la DEL</param>
void powerRoue(uint8_t vitesse, uint8_t direction, uint32_t tempsms, const uint16_t frequence)
{
    uint16_t iterations = frequence * tempsms / 1000;
	const uint16_t delai = 14;//5400 / frequence "kinda";
    const uint16_t vitesseMax = E100;
	
	if(vitesse != 0)
	{
		PORTC = (ENABLE | direction) | (ENABLE | direction)<<2;
		_delay_us(3);
	}
	
	while (iterations > 0)
	{
		for(uint32_t i = 0; i < vitesse; i++)
		{
			PORTC = (ENABLE | direction) | (ENABLE | direction)<<2;
			_delay_us(delai);
		}
			
		for(uint32_t i = 0; i < vitesseMax - vitesse; i++)
		{
			PORTC = (!ENABLE & direction) | (!ENABLE & direction)<<2;
			_delay_us(delai);
		}
		
		iterations--;
	}
}
