#define F_CPU   8000000UL 	//la vitesse du processeur
#define SORTIE  0xff		//pour definir un port entier en sortie
#define ENTREE  0x00		//pour definir un port entier en entree
#define DELAI   1000		//valeur par defaut d'un delai
#define ROUGE   0x01		//pour afficher la couleur rouge
#define VERT    0x02		//pour afficher la couleur vert
#define ETEINT	0x00		//pour afficher la couleur noir

#include <avr/io.h> 
#include <util/delay.h>	//bibliotheque contenant les methodes relatives aux delais
#include <avr/interrupt.h>: Interrupts

void intialisation();
int main();






