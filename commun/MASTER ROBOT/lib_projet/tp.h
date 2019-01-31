#define F_CPU   8000000UL 	//la vitesse du processeur
#define SORTIE  0xff		//pour definir un port entier en sortie
#define ENTREE  0x00		//pour definir un port entier en entree
#define DELAI   1000		//valeur par defaut d'un delai
#define ROUGE   0x01		//pour afficher la couleur rouge
#define VERT    0x02		//pour afficher la couleur vert
#define ETEINT	0x00		//pour afficher la couleur noir
#define AVANCER 0x00
#define RECULER 0x01
#define ENABLE  0x02

#include <avr/io.h> 
#include <util/delay.h>	//bibliotheque contenant les methodes relatives aux delais
#include <avr/interrupt.h>: Interrupts
#include <stdbool.h>

///<summary>
///affiche une jolie transition entre deux couleurs
///</summary>
///<param name ='origine'>couleur d'origine de la DEL</param>
///<param name ='cible'>couleur cible de la DEL</param>
void powerRoue(uint8_t vitesse, uint8_t direction, uint32_t tempsms, const uint16_t frequence);

///<summary>
///permet d'obtenir l'etat du bouton pressoir a l'instant appelee
///</summary>
///<returns>l'etat du bouton pressoir actuel</returns>
bool getEtatInterrupteur();

///<summary>
///permet d'obtenir l'etat stable du bouton pressoir a l'aide du debouncing
///</summary>
///<returns>l'etat du bouton pressoir actuel</returns>
bool EstActive(uint8_t pin, uint8_t position);

///<summary>
///permet de determiner si le bouton vient tout juste d'etre appuye (avec debouncing)
///</summary>
///<returns>le moment ou le bouton vient d'etre appuye</returns>
bool EstNouveauActive();

///<summary>
///affiche une jolie transition entre deux couleurs
///</summary>
///<param name ='origine'>couleur d'origine de la DEL</param>
///<param name ='cible'>couleur cible de la DEL</param>
void fadeDEL(uint8_t origine, uint8_t cible);

void ajustementRoues (uint8_t vitesseRoueGauche, uint8_t vitesseRoueDroite, uint8_t direction);

bool EstNouveauBoutons(uint8_t pin);
bool ThisIsTheShit(uint8_t pin);
