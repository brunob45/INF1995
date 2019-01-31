#include "guidage.h"
#include "can.h"

volatile int 		decisions[3] = {1,1,1};
volatile uint8_t 	valRoueGauche = ROUES_MAX, 
				valRoueDroite = ROUES_MAX, 
				ancienneDroite = 0, 
				ancienneGauche = 0,
				etat = SUIVRE_LIGNE,
				ancienEtat = CHECKPOINT;

void tournerDroite()
{
	ajustementRoues(0xff, 0x00, AVANCER);
	_delay_ms(800);
}

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
		if(EstActive(PIND, 0b01)||EstActive(PIND, 0b10))
		{
			if(PIND & 0b01)
				decisions[i] = 0;

			else if(PIND & 0b10)
				decisions[i] = 1;
			++i;
			while(EstActive(PIND, 0b01)||EstActive(PIND, 0b10));
		}
	}
	_delay_ms(1000);
}

uint8_t GetBits(uint8_t input)
{
	return	0 | 
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

void gererY(uint8_t& cptDecision, bool& virageAFaire, bool& doitRevenirAGauche)
{
	if(virageAFaire) // On choisit la bonne b
	{
		doitRevenirAGauche = !decisions[cptDecision];
		if(doitRevenirAGauche)	//tourner a droite
		ajustementRoues(VALEUR_MAX_ROUES, 0, AVANCER);
		else 	//tourner a gauche
		ajustementRoues(0, VALEUR_MAX_ROUES, AVANCER);

		_delay_ms(180);	
		ajustementRoues((VALEUR_MAX_ROUES)/1.4, (VALEUR_MAX_ROUES)/1.4, AVANCER);
		_delay_ms(1200);
			
		virageAFaire = false;
		cptDecision++;
		ancienEtat = etat;
		etat = SUIVRE_LIGNE;
	}
	else //On revient sur la ligne centrale
	{
		if(!decisions[cptDecision-1])	//tourner a droite
			ancienneDroite = ROUES_MAX;
		else 	//tourner a gauche
			ancienneGauche = ROUES_MAX;
		ancienEtat = etat;
		etat = COIN;
	}
}

int main()
{
	PORTC = ROUGE;
	
	uint8_t	cptDecision = 0,
			cptStop = 0,                      
            cptCarre = 0,
            cptCheckpoint = 0,
			cptCoinApresY = 0,
			cptCoin = 0;
	//POUR LE CAS Y
	bool virageAFaire = true;
	bool doitRevenirAGauche = false,
			nouveauCoin = true;
	
	bool nouveauBlanc = true;
	
	initialisationMaterielle();

	uint8_t refLumiere = (can().lecture(1));
		

	obtenirDecisions();
	DDRD = SORTIE;
	
	PORTC = ETEINT;

	do
	{	
		valRoueGauche = ReverseBits(PINB);	//bit 1,2,x,x,x,3,x,x
		valRoueDroite = GetBits(PINB);		//bit 5,4,x,x,x,3,x,x

		switch (etat)
		{
			case SUIVRE_LIGNE :
				if(PINB & 0b00011111) // Si il voit la ligne
				{
					if(PINB & 0b01 && PINB & 0b0100)
						cptCarre++;

					nouveauBlanc = true;
					ancienneDroite = valRoueDroite;
					ancienneGauche = valRoueGauche;	

					if ((can().lecture(1)) <= refLumiere - 20)
						etat = TUNNEL;

					if ((PINB & 0b0100) && (PINB & 0b1000) && (PINB & 0b010000) && (PINB & 0b01) && (PINB & 0b10))
					{
						if(ancienEtat == CARRE)
						{
							tournerDroite();
						}
						if (ancienEtat == CHECKPOINT)
							etat = Y;
						else
						{
							etat = CHECKPOINT;
							++cptCheckpoint;
							if (cptCheckpoint > 3)
								etat = FIN;
						}
					}
				}
				else // Si il voit pas la ligne
				{
					if((ancienneDroite & 0x80) ||(ancienneGauche & 0x80)) // on est dans un angle droit
						etat = COIN;
					else 
					{
						
						if(ancienEtat == Y) //On doit revenir sur la ligne centrale apres le Y.
						{
							etat = Y;
						}
						else 	// on est dans une intersection ou sur du tape blanc.
						{
							if (ancienEtat == CHECKPOINT)                           //Intersection
							{
								etat = INTERSECTION;
							}
						}
					}
				}
				break;
				
				
			case Y :
				gererY(cptDecision, virageAFaire, doitRevenirAGauche);
				break;
				
			case INTERSECTION :
				//PORTC = VERT;
				if(decisions[cptDecision])	//tourner a droite
					ancienneDroite = ROUES_MAX;
				else 	//tourner a gauche
					ancienneGauche = ROUES_MAX;
				cptDecision++;
				ancienEtat = etat;
				etat = COIN;
				break;
				

			case TUNNEL :
				PORTD |= ROUGE | VERT;
				if((can().lecture(1)) >= refLumiere - 3)
				{
					PORTD &= 0b11111100;
					ancienEtat = etat;
					etat = SUIVRE_LIGNE;
				}break;
				
			case CARRE :
				PORTC= VERT;
				if((valRoueDroite & 0b10000000) && (valRoueDroite & 0b00000100) && (valRoueDroite & 0b01000000))
					{
						if(nouveauCoin)
						{
							cptCarre++;
							nouveauCoin = false;
						}
						if(cptCarre >= 3)
						{
							tournerDroite();
							ancienEtat = etat;
							etat = SUIVRE_LIGNE;
						}
					}
					if(!(valRoueDroite & 0b10000000) && !(valRoueDroite & 0b01000000))
						nouveauCoin = true;
				break;
				
			case COIN:
				PORTC = ROUGE;
				cptCarre = 0;
				valRoueGauche = ROUES_ETEINTES;		// On arrete tout
				valRoueDroite = ROUES_ETEINTES;
				
				if(ancienneGauche > ancienneDroite)	// tourner a gauche
					valRoueDroite = ROUES_MAX;
				else 					// tourner a droite
					valRoueGauche = ROUES_MAX;
				
				if(PINB & 0b011111)
				{
					PORTC = ETEINT;
						
					etat = SUIVRE_LIGNE;
					
					if(ancienEtat == INTERSECTION)
						if(++cptCoin == 4)
							etat = CARRE;
					
					if(ancienEtat == Y)
						if(cptCoinApresY++ == 2)
							etat = MUSIQUE;
				} break;

			case CHECKPOINT:
				ajustementRoues((VALEUR_MAX_ROUES)/1.5, (VALEUR_MAX_ROUES)/1.5, AVANCER);
				_delay_ms(800);
				if(PINB & 0b011111)	//on croise un checkpoint
				{		
					cptCheckpoint++;
					if(ancienEtat != CHECKPOINT)
					{	
						cptCoin = 0;
						ancienEtat = etat;
						etat = SUIVRE_LIGNE;
					}
					else
					{
						etat = Y;
					}
				}
				else	//on croise une fin d'intersection genre
				{
					if(decisions[cptDecision-1])	//tourner a droite
						ancienneDroite = ROUES_MAX;
					else 	//tourner a gauche
						ancienneGauche = ROUES_MAX;

					etat = COIN;
				} break;

			case MUSIQUE :

				if(!(PINB & 0x1f))
				{					
					if (nouveauBlanc)
					{
						++cptStop;
						nouveauBlanc = false;
					}	
				}
				else
					nouveauBlanc = true;

				if(cptStop >= 3)
					jouerNote(50);
				
				if(cptStop >= 7)
				{
					jouerNote(0);
					etat = SUIVRE_LIGNE;
				} break;
		
		}
		ajustementRoues((VALEUR_MAX_ROUES - valRoueGauche)/1.2, (VALEUR_MAX_ROUES - valRoueDroite)/1.2, AVANCER);
		
	} while(etat != FIN) ;

	_delay_ms(100);
	ajustementRoues(0,0,0);
	PORTC = VERT;
	return 0;
}
