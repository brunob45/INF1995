/*
 * Nom: compteur 32 bits
 * Copyright (C) 2005 Matthew Khouzam
 * License http://www.gnu.org/copyleft/gpl.html GNU/GPL
 * Description: Ceci est un exemple simple de programme 
 * Version: 1.1
 */

#include "../../tp.h"
#include "../../del.c"

int main()
{
  DDRA = SORTIE; // PORT A est en mode sortie
  DDRB = SORTIE; // PORT B est en mode sortie
  DDRC = SORTIE; // PORT C est en mode sortie
  DDRD = ENTREE; // PORT D est en mode entree.

  uint32_t cptTemps = 0;

  for(;;)  // boucle sans fin
  {
        for(cptTemps = 0; cptTemps < 500000; cptTemps++)
        {
          couleur++;
          PORTA = couleur;
        }
    
        PORTA = ROUGE;
        _delay_ms(DELAI);
        PORTA = VERT;
        _delay_ms(DELAI);
  }
  return 0; 
}

