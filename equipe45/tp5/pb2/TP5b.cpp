// Fichier : TP5b.cpp
// Auteurs : Bruno Bousquet & Dominique Deschenes
// Date    : 8 fevrier 2016
// But     : Petit jeu de reflexes

#include <../../tp.h>

volatile uint8_t minuterieExpiree = 0;
volatile uint8_t boutonPoussoir = 0; 

ISR (TIMER1_COMPA_vect) //interruption timer
{
	minuterieExpiree = 1;
}

ISR (INT0_vect) // interruption bouton
{
	int bouton = PORTD & 0x04;
	_delay_ms(20);
	if (bouton == (PORTD & 0x04))
		boutonPoussoir = 1;
}

void initialisation()
{
	cli();
	DDRA = SORTIE; //On definit les ports A en sortie
	DDRB = SORTIE; //On definit les ports B en sortie
	DDRC = SORTIE; //On definit les ports C en sortie
	DDRD = ENTREE; //On definit les ports D en entree
   	// cette procédure ajuste le registre EIMSK
	// de ATmega324PA pour permettre les interruptions externes
	EIMSK |= _BV(INT0);
	// il faut sensibiliser les interruptions externes aux
	// changements de niveau du bouton-poussoir
	// en ajustant le registre EICRA
	EICRA |= _BV(ISC00) | _BV(ISC01);
	// sei permet de recevoir a nouveau des interruptions.
	sei ();	
}

void partirMinuterie ( uint16_t duree ) 
{
	// mode CTC du timer 1 avec horloge divisee par 1024
	// interruption apres la duree specifiee
	
	
	//INITIALISATION RANDOM A FIN DE TEST
	TCNT1 = 0;							//valeur de depart
	OCR1A = duree;						//nombre d'iterations
	TCCR1A |= _BV(WGM12)| _BV(COM1A1);  //Set min & max
	TCCR1B |= _BV(CS12) | _BV(CS10); 	//CS12 & CS10 pour /1024
	TCCR1C |= 0;
	TIMSK1 |= _BV(OCIE1A);//|_BV(ICIE1);	//timer mask
}

int main()
{  
	initialisation(); 
	_delay_ms(10000);
	PORTA = ROUGE;
	_delay_ms(50);
	PORTA = ETEINT;
	partirMinuterie(1000); //(8 000 000 / 1024)-1 = 7811
	while ( minuterieExpiree == 0 && boutonPoussoir == 0 ); 
		// attendre qu'une des deux variables soit modifiee
		// par une ou l'autre des interruptions.
	
	// Une interruption s'est produite. Arreter toute
	// forme d'interruption. Une seule reponse suffit.
	
	cli();

	if (boutonPoussoir == 1)
		PORTA = VERT;
	else
		PORTA = ROUGE;

	return 0;
}
