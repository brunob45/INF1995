#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>

void delay(uint16_t delai) // Une fonction pour le delai
{
    for(uint16_t i = 0; i < delai; i++)
    _delay_us(10);
}

int main()
{
 
  DDRB = 0xff;      // PORT B est en mode sortie

  
  
  for(;;)             // boucle sans fin
  {
    uint16_t a = 540;
    uint16_t b = 0;
   
    for(int i = 0; i < 540; i++)
    {
        a = a -1;
        b = b + 1;
        
        PORTB = 0b00000001;
        delay(a);
        PORTB = 0b00000000;
        delay(b);
    }
    
    a = 540;
    b = 0;
    
    for(int i = 0; i < 540; i++)
    {
        a = a -1;
        b = b + 1;
        
        PORTB = 0b00000010;
        delay(a);
        PORTB = 0b00000000;
        delay(b);
    }
   
}
}