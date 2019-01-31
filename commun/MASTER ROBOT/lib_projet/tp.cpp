#include "tp.h"

enum vitesses {E0, E25, E50, E75, E100};
volatile bool etatAncien = false; //variable contenant l'ancien etat du bouton poussoir

///<summary>
///permet d'obtenir l'etat du bouton pressoir a l'instant appelee
///</summary>
///<returns>l'etat du bouton pressoir actuel</returns>
bool getEtatInterrupteur(uint8_t pin, uint8_t position)
{
	return pin & position;
}

///<summary>
///permet d'obtenir l'etat stable du bouton pressoir a l'aide du debouncing
///</summary>
///<returns>l'etat du bouton pressoir actuel</returns>
bool EstActive(uint8_t pin, uint8_t position)
{
    bool etat = getEtatInterrupteur(pin, position); //lecture de l'etat du bouton
	
    if(etat == etatAncien) //si l'etat ne change pas, on le laisse intact
    {
        return etat;
    }
    else 					//sinon
    {
        _delay_ms(10);		//on attend un delai raisonnable
        etatAncien = etat;	
        etat = getEtatInterrupteur(pin, position);	//on refait une lecture
        
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
bool EstNouveauActive(uint8_t pin, uint8_t position)
{
	bool etat = getEtatInterrupteur(pin, position); //lecture de l'etat du bouton
	
    if(etat == etatAncien)	//si l'etat ne change pas, on le laisse intact
    {                       
        return false;       
    }                       
    else                    //sinon
    {                       
        _delay_ms(200);      //on attend un delai raisonnable
        etatAncien = etat;  
        etat = getEtatInterrupteur(pin, position);	//on refait une lecture
                            
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

bool ThisIsTheShit(uint8_t pin)
{
	bool etat = EstActive(pin, 0x01) || EstActive(pin, 0x02);
	return etat && !etatAncien;
}

bool EstNouveauBoutons(uint8_t pin)
{
	return (EstNouveauActive(pin, 0x01) | EstNouveauActive(pin, 0x02));
}

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

void ajustementRoues (uint8_t vitesseRoueGauche, uint8_t vitesseRoueDroite, uint8_t direction) 
{
	OCR1A = vitesseRoueDroite ;
	OCR1B = vitesseRoueGauche ;
	PORTD |= direction << 2 | direction << 3;
}
