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