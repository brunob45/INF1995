/* Fichier : TP7b.cpp
 * Auteurs : Bruno Bousquet et Dominique Deschênes
 * Date	   : 25 février 2016
 * But     : Petit programme qui colore la DEL proportionellement à l'intensitée lumineuse 
 * 		lue par le CAN.
 
-/!\- BRANCHEMENTS -/!\-
 * Alimentation Breadboard : Fils rouge et noir sur VCC et GND.
 * Connexion photoresistance : Fil bleu sur pin 1 du PORTA.
 * Connexion DEL : (+) -> PORTC 1, (-) -> PORTC 2.
 */

#include "tp.h"
#include "can.cpp"

// Applique les bons paramètres aux ports I/O.
void initialisation()
{	
	DDRA = SORTIE; // On definit les ports A en sortie.
	DDRB = SORTIE; // On definit les ports B en sortie.
	DDRC = SORTIE; // On definit les ports C en sortie.
	DDRD = SORTIE; // On definit les ports D en sortie.
}

int main()
{
	/* Valeur du décalage de la lecture du CAN pour amplifier le signal.
	 * Sinon la luminosité ambiante n'est pas assez pour faire une couleur
	 * ambrée.  */
	const uint8_t amplification = 5;

	// Valeur où est stockée la lecture du CAN.
	uint8_t bronzage = 0;
	// Valeur du compteur pour la couleur.
	uint8_t compteur = 0;
	initialisation();
	
	// On fait la boucle indéfiniment.
	for(;;)
	{
		// Effectue la lecture du CAN.
		bronzage = can().lecture(0) << (amplification - 2);

		/* De 0 à bronzage, la DEL allume rouge : plus bronzage est grand, 
		 *	plus la lumière sera rouge longtemps.
		 * On remet le compteur = 0.  */
		for(compteur = 0; compteur < bronzage; compteur++)
			PORTC = ROUGE;
		
		/* De bronzage à valeur max, la DEL allume vert : plus bronzage 
		 *	est petit plus la lumière sera verte longtemps.  */
		for(; compteur < 0xff; compteur++)
			PORTC = VERT;
	}
	
	return 0;
}
