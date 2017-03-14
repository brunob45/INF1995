#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>
#include "memoire_24.h"

#define SORTIE 0XFF


void initialisationUART () 
{
	// 2400 bauds. Nous vous donnons la valeur des deux
	// premier registres pour vous éviter des complications
	UBRR0H = 0;
	UBRR0L = 0xCF;
	// permettre la reception et la transmission par le UART0
	UCSR0A = _BV(UDRE0);
	UCSR0B = _BV(RXEN0) | _BV(TXEN0);
	// Format des trames: 8 bits, 1 stop bits, none parity
	UCSR0C = _BV(UCSZ01) | _BV(UCSZ00);
}


void initialisation()
{	
	DDRA = SORTIE; //On definit les ports A en sortie
	DDRB = SORTIE; //On definit les ports B en sortie
	DDRC = SORTIE; //On definit les ports C en sortie
	DDRD = SORTIE; //On definit les ports D en sortie
	initialisationUART () ;
}


// De l'USART vers le PC
void transmissionUART ( uint8_t donnee ) 
{
	//Wait for empty transmit buffer 
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = donnee;
}



// Du PC a l USART
uint8_t receptionUART()
{
 
    while(!(UCSR0A & (1 << RXC0)));
        return UDR0 ;
}
/*
void envoyer()
{
	uint8_t lettre;
	for(uint8_t j = 0; j < 0xff; j++)
	{
		Memoire24CXXX().ecriture(j, lettre);
			receptionUART(lettre);
	}
}
*/
int main()
{
	initialisation();
        
	/*PORTA = ROUGE;
	envoyer();
	PORTA = VERT;*/
        
        uint8_t taille1 ;;
        uint8_t taille2 ;;
        Memoire24CXXX().init();
       
        Memoire24CXXX().lecture(0x00, &taille1);
        _delay_ms(5);
        Memoire24CXXX().lecture(0x01, &taille2);
         _delay_ms(5);
         
         uint16_t nbInstructions = ((taille1 << 8) | taille2 )- 0x02;
         transmissionUART(taille1);
         transmissionUART(taille2);
        
         if (nbInstructions == 34) PORTA = 0X01;
       for(uint8_t j = 0; j < nbInstructions; j++)
	{
                uint8_t donnee ;
		Memoire24CXXX().lecture(0x02 + j, &donnee);
                 _delay_ms(5);
		transmissionUART(donnee);	
	}
        
	return 0;
        
        
        
       
}


