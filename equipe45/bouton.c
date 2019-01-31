volatile bool etatAncien = false; //variable contenant l'ancien etat du bouton poussoir

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
