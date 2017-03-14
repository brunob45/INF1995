#include "tp.h"

enum vitesses {E0, E25, E50, E75, E100};
volatile bool etatAncien = false; //variable contenant l'ancien etat du bouton poussoir
//wassup
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

///<summary>
///permet d'obtenir l'etat du bouton pressoir a l'instant appelee
///</summary>
///<returns>l'etat du bouton pressoir actuel</returns>
bool getEtatInterrupteur()
{
	return PIND & 0x04;
}

///<summary>
///permet d'obtenir l'etat stable du bouton pressoir a l'aide du debouncing
///</summary>
///<returns>l'etat du bouton pressoir actuel</returns>
bool EstActive()
{
    bool etat = getEtatInterrupteur(); //lecture de l'etat du bouton
	
    if(etat == etatAncien) //si l'etat ne change pas, on le laisse intact
    {
        return etat;
    }
    else 					//sinon
    {
        _delay_ms(10);		//on attend un delai raisonnable
        etatAncien = etat;	
        etat = getEtatInterrupteur();	//on refait une lecture
        
        if(etat == etatAncien)	//si l'etat a effectivement change
        {
			return etatAncien;	//on retourne l'etat mis a jour
		}
		else 					//sinon
		{
			etatAncien = etat;	//on remet l'etat ancien a sa valeur originale
			return etat;		//on retourne l'etat ancien
		}
    }
}

///<summary>
///permet de determiner si le bouton vient tout juste d'etre appuye (avec debouncing)
///</summary>
///<returns>le moment ou le bouton vient d'etre appuye</returns>
bool EstNouveauActive()
{
	bool etat = getEtatInterrupteur(); //lecture de l'etat du bouton
	
    if(etat == etatAncien)	//si l'etat ne change pas, on le laisse intact
    {                       
        return false;       
    }                       
    else                    //sinon
    {                       
        _delay_ms(10);      //on attend un delai raisonnable
        etatAncien = etat;  
        etat = getEtatInterrupteur();	//on refait une lecture
                            
        if(etat == etatAncien)	//si l'etat a effectivement change
        {                   
			return etatAncien;	//on retourne l'etat mis a jour
		}                   
		else                	//sinon
		{                   
			etatAncien = etat;	//on remet l'etat ancien a sa valeur originale
			return etat;    	//on retourne l'etat ancien
		}
    }
}
