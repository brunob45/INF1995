#include "../../tp.h"

void initialisation()
{
	DDRD = SORTIE;
	//Sorties ENABLE des roues sur PORTD[4:5]
	
	// mise a un des sorties OC1A et OC1B sur comparaison
	// reussie en mode PWM 8 bits, phase correcte
	// et valeur de TOP fixe a 0xFF (mode #1 de la table 16-5
	// page 130 de la description technique du ATmega324PA)
	TCCR1A = _BV(WGM10) | 
			/* WGMn2*	WGMn1	WGMn0	Mode	TOP		Dessin
			 *>0		0		1		PWM		0xFF	.-'-.-'
			 * 1		0		0		CTC		OCR1A	
			 * 1		0		1		FastPWM	0xFF	.-'.-'.-'
			 * [...]
			 * (* Se trouve dans TCCR1B)				*/

		_BV(COM1A1) | _BV(COM1B1);
			/* COM1A1/B1	COM1A0/B0	Desc (x=A,B) (with Fast PWM)
			 * 0			0			No output
			 * 0			1			Toggle OC1A, OC1B disconnected
			 *>1			0			Clear at compare match, set at BOTTOM
			 * 1			1			Set at compare match, clear at BOTTOM	 
			 * [...]						*/
	
	// division d'horloge par 8 - implique une frequence de PWM fixe
	TCCR1B = _BV(CS11) ; 
			/* CS12		CS11	CS10	Prescaler
			 * 0		0		0		0 (No clock source)
			 * 0		0		1		1 (No prescaler)
			 *>0		1		0		8
			 * 0		1		1		64
			 * 1		0		0		256
			 * 1		0		1		1024	*/
	 
	TCCR1C = 0; //Utile seulement lorsque le timer est pas PWM.
}

void ajustementPWM (uint8_t vitesseRoueGauche, uint8_t vitesseRoueDroite) 
{
	OCR1A = vitesseRoueDroite ;
	OCR1B = vitesseRoueGauche ;
}

int main ()
{
	initialisation();
	for(;;)
	{	
		ajustementPWM(0xFF*.25, 0xFF*.25);
		_delay_ms(DELAI*2);
		ajustementPWM(0xFF*.5, 0xFF*.5);
		_delay_ms(DELAI*2);
		ajustementPWM(0xFF*.75, 0xFF*.75);
		_delay_ms(DELAI*2);
		ajustementPWM(0xFF, 0xFF);
		_delay_ms(DELAI*2);
	}
	return 0;
}
