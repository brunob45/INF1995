#define F_CPU  8000000UL //la vitesse du processeur
#define SORTIE 0xff		//pour definir un port entier en sortie
#define ENTREE 0x00		//pour definir un port entier en entree
#define ROUGE  0x01		//pour afficher la couleur rouge
#define VERT   0x02		//pour afficher la couleur vert
#define ETEINT 0x00		//pour eteindre la lumiere
#define DELAI  1000		//valeur par defaut d'un delai

#include <avr/io.h> 
#include <util/delay.h>	//bibliotheque contenant les methodes relatives aux delais

bool etatAncien = false; //variable contenant l'ancien etat du bouton poussoir

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
