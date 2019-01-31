/*
 INF1995: TP7 probleme1
 Auteurs  JULIO H. DANDJINOU et J. Staelle MASSADO FOKO (groupe 63)
 Date: 28/ 02/ 2016
 Description: Ce programme permet de mettre une minuterie en marche une fois que le bouton poussoir monte sur le breadbord est appuye.
               
               
  BRANCHEMENT des cables:
      - Nous utilisons le PORTA(PORTA1 en particulier) pour allumer la LED.
      - Nous utilisons le PORTD(VCC et GND) pour l'alimentation du breadboard.
      - Pour le cable de donnees, nous utilisons PD2 (PORTD 2) comme demande dans l'enonce.
*/



    #include <avr/io.h>
    #define  F_CPU 8000000UL
    #include <util/delay.h>
    #include<avr/interrupt.h>
    #include<avr/sleep.h>
    
    
        // Fonction peser pour gerer l'antirebond
        bool peser()
        {
            bool estpese = false; //Initialisation de la valeur de retour
            if (PIND & 0x04 ) // permet de verifier si le bouton poussoir est pese.
            {
            _   delay_ms(10);
                if (PIND & 0x04 ) // Une deuxieme verification pour savoir si le bouton poussoir est pese.
                {
               
                    estpese = true;
                }
            }  
                return estpese;
        }
    
            volatile uint8_t minuterieExpiree;
            volatile uint8_t compteur =0;
    
    
        void initialisation ( void ) {

            // cli est une routine qui bloque toutes les interruptions.

            // Il serait bien mauvais d'etre interrompu alors que

            // le microcontroleur n'est pas pret...

            cli ();


            // configurer et choisir les ports pour les entrees

            // et les sorties. DDRx... Initialisez bien vos variables

            DDRD = 0x00;                                    // portD en entree
            DDRA = 0xff;                                    // portA en sortie


            // cette procédure ajuste le registre EIMSK

            // de ATmega324PA pour permettre les interruptions externes

            EIMSK |= _BV(INT0);


            // il faut sensibiliser les interruptions externes aux

            // changements de niveau du bouton-poussoir

            // en ajustant le registre EICRA

            EICRA |= (1 << ISC00) ;


            // sei permet de recevoir a nouveau des interruptions.

            sei ();

        }

        //Cette fonction permet d'ajuster la valeur des differents registre pour le foncionnement du timer1
        void partirMinuterie ( uint16_t duree ) {
            minuterieExpiree = 0;
            // mode CTC du timer 1 avec horloge divisee par 1024
            // interruption apres la duree specifiee
            TCNT1 = 0 ;
            OCR1A = duree;
            TCCR1A |= (1<< WGM01) ;
            TCCR1B |= (1 << CS12 ) | (1 << CS10) ;
            TIMSK1 = (1 << OCIE1A);         
        }


        //Cette  routine d'interruption est declanchee chaque fois qu'une dixieme de seconde s'ecoule
        ISR ( TIMER1_COMPA_vect ) {
            if(compteur<120) 
                compteur++;
            if(compteur==120 || peser()) 
            {
                minuterieExpiree =1;
            }
        }

        //Cette routine d'interruption se declanche quand le boutton poussoir est appuyer
        ISR ( INT0_vect ) {

            if (!peser())
            {
                partirMinuterie(781);     // 781 a ete trouve en faisant le calcul suivant: (8000000/1024)/10     
            }
            EIFR = (1 << INTF0);
        }

    
    int main(){

    
    
        initialisation();       // appel de la fonction d initialisation                 
            
        do
         {        
                //attendre que la minuterie soit expiree ou que le boutton soit relacher
         } while ( minuterieExpiree == 0);
         // Une interruption s'est produite. Arreter toute
         // forme d'interruption. Une seule reponse suffit.
         cli ();
        
         //On fait clignoter la lumiere verte pendant 1/2 seconde
         for(uint8_t i = 0; i < 5; i++)
                {
                    PORTA = 0x01;  
                    _delay_ms(50);
                    PORTA = 0;
                    _delay_ms(50);
                }
        
         //On eteind la LED pendant 2 secondes
         PORTA = 0;
         _delay_ms(2000);
           
         //On fait clignoter la lumiere rouge compteur/2 fois avec une frequence de 2 fois par seconde   
         for (uint8_t k = 0; k <(compteur/2); k++)
        {
                    for (uint8_t l= 0; l<2; l++)
                    {
                        PORTA = 0x02;
                        _delay_ms(250);
                        PORTA = 0;
                        _delay_ms(250);
                    }
        }
           
         //On allume la LED en vert pour une seconde 
         PORTA = 0x01;
         _delay_ms(1000);
         
         //Et enfin on eteind la LED
         PORTA = 0;   
    
  
        return 0;
    }
