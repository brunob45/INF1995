/************************* BRANCHEMENTS **************************
 *								 *
 *	ROUES	ENABLE -> PORTD 5,6	DIRECTION -> PORTD 3,4	 *
 *	PIEZO	PORTD7,8					 *
 *	DEL	(+) -> PORTA1		(-) -> PORTA2		 *
 * 	ProgEn	VCC -> PINA8					 *
 *****************************************************************/

#include "projet.h"

int main()
{
	uint8_t etatSensor, droit, gauche;
	initialiser();
	
	while(1)
	{
		etatSensor = getEtatSensor();
		droit = getDroit(etatSensor);
		gauche = getGauche(etatSensor);
		
		if(gauche > droit)
			ajustementRoues(0xff, 0x00, AVANCER);
		else if (gauche < droit)
			ajustementRoues(0x00, 0xff, AVANCER);
		else
			ajustementRoues (0xff, 0xff, AVANCER);
	}
}

void initialiser() {
	DDRA = ENTREE;
	DDRB = SORTIE;
	DDRC = SORTIE;
	DDRD = SORTIE;
	
	//PWM Roues
	TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
	TCCR1B = _BV(CS11) ; 
	TCCR1C = 0;
	
	initialisationUART();
}

uint8_t getEtatSensor()
{
	return PINA;
}

uint8_t getPin(uint8_t pin, uint8_t numero)
{
	return ( pin >> (numero-1) ) & (0x01);
}

uint8_t getGauche(uint8_t pin)
{
	uint8_t data = 0x00;
	data &= getPin(1) << 2;
	data &= getPin(2) << 1;
	data &= getPin(3)
	return data;
}

uint8_t getDroit(uint8_t pin)
{
	uint8_t data &= (pin >> 2) & 0x08;
	return data;
}
