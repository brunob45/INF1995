#include <avr/io.h> 
#define F_CPU 8000000
#include <util/delay.h>

/*

    ******************************************************
    * Etat présent *  Entrée  * Prochain état * Sortie   *
    ******************************************************
    *   Etat0      *    0     *      Etat0    * rouge    *
    ******************************************************
    *   Etat0      *    1     *      Etat1    * Ambre    *        
    ******************************************************                              
    *   Etat1      *    0     *      Etat2    * vert     *           
    ******************************************************                         
    *   Etat1      *    1     *      Etat1    * ambre    *         
    ******************************************************                         
    *   Etat2      *    0     *      Etat2    * vert     *            
    ******************************************************                       
    *   Etat2      *    1     *      Etat3    * rouge    *           
    ******************************************************
    *   Etat3      *    0     *      Etat4    * eteint   *             
    ******************************************************                    
    *   Etat3      *    1     *      Etat3    * rouge    *
    ******************************************************
    *   Etat4      *    0     *      Etat4    * eteint   *
    ******************************************************
    *   Etat4      *    1     *      Etat5    * vert     *
    ******************************************************
    *   Etat5      *    0     *      Etat0    * rouge    *
    ******************************************************
    *   Etat5      *    1     *      Etat5    * vert     *
    ******************************************************

            Table 2: Tableau des états
            
            
Les entrees 0 et 1 correspondent respectivement a "bouton-poussoir relache" et "bouton-poussoir pese".            
La sortie correspond a la couleur de la DEL.

            
            
*/


// Fonction peser pour gerer l'antirebond
    bool peser()
    {
        bool estpese = false; //Initialisation de la valeur de retour
        if (PIND & 0x04 ) // permet de verifier si le bouton poussoir est pese.
        {
            _delay_ms(10);
            if (PIND & 0x04 ) // Une deuxieme verification pour savoir si le bouton poussoir est pese.
            {
               
            estpese = true;
            }
        }  
        return estpese;
    }
    
    
void fonctionRouge() // Fonction pour que la DEL s'allume rouge
{
    PORTB = 0b00000010;
}


void fonctionAmbre() // Fonction pour que la DEL s'allume ambre
{
{
    
    PORTB = 0b00000010;
    _delay_ms(10);
    PORTB = 0b00000001;
    _delay_ms(10);
    
    
}

void fonctionVert() // Fonction pour que la DEL s'allume vert
{
{
    PORTB = 0b00000001;
}

void fonctionEteint() // Fonction pour que la DEL soit eteinte
{
{
    PORTB = 0b00000000;
}




int main()
{
    
  enum Etat {Etat0, Etat1, Etat2, Etat3, Etat4, Etat5 } ;// Les differents etats
  
  int etatpresent = Etat0;
     
  DDRB = 0xff; // PORT B est en mode sortie
  DDRD = 0x00; // PORT D est en mode entree
  
  
  PORTB = 0b00000010; //Initialisation de la DEL a lacouleur rouge
  
  
for(;;)  // boucle sans fin
{
   
    
    switch(etatpresent)
    {
        case Etat0:
            
                    if (peser())
                    {
                            
                       etatpresent = Etat1;  // Passe de l'etat Etat0 a Etat1 si on pese sur le bouton.
                    }
                    else
                        fonctionRouge();
                    
                    break;
        case Etat1:
                   
                    if (!peser())   // Passe de l'etat Etat1 a Etat2 quand on relache le bouton .
                    {
                        etatpresent = Etat2;
                        fonctionVert();  
                    }
                    else
                        fonctionAmbre();
                    
                    break;
        
        case Etat2:
                    if (peser())    //  Passe de l'etat Etat2 a Etat3 si on pese sur le bouton.
                    {
                       
                          
                        etatpresent = Etat3;
                        fonctionRouge();
                    }
                    else
                        fonctionVert();
                    
                    break;
        
        case Etat3:
                    
                    if (!peser())
                    {
                        fonctionEteint();
                        etatpresent = Etat4;
                         
                    }
                    else
                        fonctionRouge();
                    
                    break;
        
        case Etat4:
                    if (peser())    // Passe de l'etat Etat4 a Etat5 si on pese sur le bouton.
                    {
                        fonctionVert();
                        etatpresent = Etat5;
                        
                    }
                     else 
                         fonctionEteint();
                         
                    break;
            
                    
        case Etat5: 
                    
                    if (!peser())    // Passe de l'etat Etat5 a Etat0 quand on relache le bouton .
                    {
                        fonctionRouge();
                        etatpresent = Etat0;
                        
                    }
                    else
                        fonctionVert();
                     
                    break;
            
    }
}
  return 0; 
}