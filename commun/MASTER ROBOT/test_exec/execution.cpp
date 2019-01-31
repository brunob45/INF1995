#include "guidage.h"
#include "can.h"

volatile int decisions[3] = {1,1,1};
volatile uint8_t valRoueGauche = ROUES_MAX, 
			valRoueDroite = ROUES_MAX, 
			ancienneDroite = 0, 
			ancienneGauche = 0,              
                        cptCarre = 0,
			etat = 0,
			ancienEtat = 0;
volatile bool nouveauBlanc = true;

void initialisationMaterielle()
{
	DDRA = ENTREE;
	DDRB = ENTREE;
	DDRC = SORTIE;
	DDRD = 0b11111100;
	
	//CTC Piezo
	TCCR2A = _BV(WGM21) | _BV(COM2A0);
	TCCR2B = _BV(CS02); 
	OCR2A = 0;

	 //PWM Roues
	TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
	TCCR1B = _BV(CS11) ; 
	TCCR1C = 0;
	
}

void obtenirDecisions()
{
	for (int i = 0; i<3;)
	{
		if(EstNouveauBoutons(PIND))
		{
			if(PIND & 0b01)
				decisions[i] = 0;
			else if(PIND & 0b10)
				decisions[i] = 1;
			++i;
		}
	}
	DDRD = SORTIE;
	_delay_ms(1000);
}

uint8_t GetBits(uint8_t input)
{
	return 	0 | 
			((input & 0b00011000) << 3) |
			(input & 0b00000100);
}

uint8_t ReverseBits(uint8_t input)
{
	return 	0|
			((input & 0b00000001) << 7) |
			((input & 0b00000010) << 5) | 
			((input & 0b00000100));
}

void jouerNote(uint8_t code)
{ 
	if(code == 0)
		OCR2A = 0;
	else
		OCR2A = 1911 * pow(2.7, -0.058 * code); 
}

void gererY(uint8_t cptDecision)
{
	bool doitRevenirAGauche = false;
	
	doitRevenirAGauche = decisions[cptDecision];
	if(doitRevenirAGauche)	//tourner a droite
	{
		if(!(PINB & 0b00000011))
			valRoueDroite = ROUES_ETEINTES;
		else
			valRoueDroite = ROUES_MAX;
		// on est rendu au bout	
		if(!(PINB & 0b00011111))
			ancienneGauche = ROUES_MAX;
	}
	else
	{
		if(!(PINB & 0b00011000))
			valRoueGauche = ROUES_ETEINTES;
		else
			valRoueGauche = ROUES_MAX;
		
		// on est rendu au bout
		if(!(PINB & 0b00011111))
			ancienneDroite = ROUES_MAX;
	}
	
	/*
	if(virageAFaire) // On choisit la bonne b
	{
		doitRevenirAGauche = decisions[cptDecision];
		if(doitRevenirAGauche)	//tourner a droite
			valRoueDroite = ROUES_ETEINTES;
		else 	//tourner a gauche
			valRoueGauche = ROUES_ETEINTES;
			
		virageAFaire = false;
		cptDecision++;
		ancienEtat = Y;
		etat = SUIVRE_LIGNE;
	}
	else //On revient sur la ligne centrale
	{
		if(doitRevenirAGauche)
		{
			valRoueGauche = ROUES_ETEINTES; 
		}
		else
		{
			valRoueDroite = ROUES_ETEINTES;
		}
		etat = SUIVRE_LIGNE;
	}*/

}

void gererCarre()
{
	while(cptCarre < 5 )
        {
		valRoueGauche = ReverseBits(PINB);
                valRoueDroite = GetBits(PINB);
                                  
                if((PINA & 0b00010000) && (PINA & 0b00001000) && (PINA & 0b00000100) && !(PINA & 0b0000001))
 			cptCarre++;
                                    
 		if(cptCarre == 2 || (PINB & 0b00011111)) //premier tournant du carre ou dernier tournant du carre       
		{
                        valRoueGauche = ROUES_MAX; // Tourner a droite
			ancienEtat = etat;
			etat = SUIVRE_LIGNE;
		}        

                ajustementRoues((VALEUR_MAX_ROUES - valRoueGauche)/1, (VALEUR_MAX_ROUES - valRoueDroite)/1, AVANCER);
	}    
}

void definirDirection(uint8_t &cptStop, uint8_t &cptDecision, uint8_t &refLumiere)
{
	switch (etat)
	{
		case SUIVRE_LIGNE :
			if(PINB & 0b00011111)
			{
				PORTC= ROUGE;
				PORTC= VERT;
				nouveauBlanc = true;
				ancienneDroite = valRoueDroite;
				ancienneGauche = valRoueGauche;	
				/* Ci-dessous casse! */
				/*if((PINB & 0b00010000) == 0b00010000 && 
				   (PINB & 0b00000001) == 0b00000001 && 
				   (!(PINB & 0b00000010) | !(PINB & 0b00000100) | !(PINB & 0b00001000))); //Y
				{
					ancienEtat = SUIVRE_LIGNE;
					etat = Y;
					break;
				}*/
			}
			else
			{
				valRoueGauche = ROUES_ETEINTES;
				valRoueDroite = ROUES_ETEINTES;
				
				if(!(ancienneDroite & 0x80 || ancienneGauche & 0x80)) // on est dans un angle droit
				{
					if (nouveauBlanc)
					{
						++cptStop;
						nouveauBlanc = false;
					}
					if(ancienEtat != Y) //On doit revenir sur la ligne centrale apres le Y.
					{
						if ((cptStop - cptDecision) > 0) //Ligne pointille
							jouerNote(50);
						else                            //Intersection
							jouerNote(0);
					}
				}
			}
			break;
			
		case Y :
			gererY(cptDecision);
			break;
			
		case INTERSECTION :
			if(decisions[cptDecision])	//tourner a droite
				ancienneDroite = ROUES_MAX;
			else 	//tourner a gauche
				ancienneGauche = ROUES_MAX;
			cptDecision++;
			
			/*if(decisions[cptDecision])	//tourner a droite
				valRoueDroite = ROUES_ETEINTES;
			else 	//tourner a gauche
				valRoueGauche = ROUES_ETEINTES;
			cptDecision++;
			etat = SUIVRE_LIGNE;
			ancienEtat = INTERSECTION;*/
			break;
			

		case TUNNEL :
			PORTD |= ROUGE | VERT;
			if((can().lecture(1) >> 2) >= refLumiere -1)
			{
				PORTD &= 0b11111100; // Éteindre les lumières.
			}
			break;
			
		case CARRE :
			gererCarre();
			break;
			
		case COIN:
			// On arrete tout
			valRoueGauche = ROUES_ETEINTES;
			valRoueDroite = ROUES_ETEINTES;
			
			if(ancienneGauche > ancienneDroite)	// tourner a gauche
				valRoueDroite = ROUES_MAX;
			else 								// tourner a droite
				valRoueGauche = ROUES_MAX;
			break;
	
	}
}

void choisirProchainEtat(uint8_t &refLumiere, uint8_t &cptStop, uint8_t &cptDecision)
{
	switch (etat)
	{
		case SUIVRE_LIGNE :
			if(PINB & 0b00011111)
			{
				if ((can().lecture(1) >> 2) <= refLumiere - 5)
					etat = TUNNEL;
			}
			else
			{
				if(ancienneDroite & 0x80 || ancienneGauche & 0x80) // on est dans un angle droit
				{
					ancienEtat = etat;
					etat = COIN;
				}
				else 
				{
					if(ancienEtat == Y) //On doit revenir sur la ligne centrale apres le Y.
					{
						ancienEtat = SUIVRE_LIGNE;
						etat = Y;
					}
					else 	// on est dans une intersection ou sur du tape blanc.
					{
						if ((cptStop - cptDecision) <= 0)//Intersection
						{
							etat = INTERSECTION;
						}
					}
				}
			}
			break;
			
			
		case Y :
			if(!(PINB & 0b00011111))
			{
				ancienEtat = etat;
				etat = COIN;
			}
			break;
			
		case INTERSECTION :
			ancienEtat = etat;
			etat = COIN;
			break;
			
		case TUNNEL :
			if((can().lecture(1) >> 2) >= refLumiere -1)
			{
				ancienEtat = etat;
				etat = SUIVRE_LIGNE;
			}
			break;
			
		case CARRE :
			break;
			
		case COIN:
			if(PINB & 0b011111)
			{
				ancienEtat = etat;
				etat = SUIVRE_LIGNE;
			}
			break;
	}
}

int main()
{
	PORTC = ROUGE;
	
	uint8_t	cptDecision = 0,
			cptStop = 0;
	//POUR LE CAS Y
	//bool virageAFaire = true;
	//bool doitRevenirAGauche = false;

	
	initialisationMaterielle();
	//PORTC = ROUGE;
	//obtenirDecisions();
	//PORTC = VERT;
	DDRD = SORTIE;
	
	uint8_t refLumiere = (can().lecture(1) >> 2);
	etat = SUIVRE_LIGNE;
	
	PORTC = VERT;
	do
	{	
		valRoueGauche = ReverseBits(PINB);	//bit 1,2,x,x,x,3,x,x
		valRoueDroite = GetBits(PINB);		//bit 5,4,x,x,x,3,x,x
				
		definirDirection(cptStop, cptDecision, refLumiere);
		choisirProchainEtat(refLumiere, cptStop, cptDecision);	
		
		ajustementRoues((VALEUR_MAX_ROUES - valRoueGauche)/1, (VALEUR_MAX_ROUES - valRoueDroite)/1, AVANCER);
		
		
	} while(etat != FIN) ;
	ajustementRoues(0,0,0);
	PORTC = VERT;
	return 0;
}

