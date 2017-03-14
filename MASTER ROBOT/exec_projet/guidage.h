#include "tp.h"
#define VALEUR_MAX_ROUES 0xff
#define ROUES_ETEINTES 0xff//0b11110100
#define ROUES_MAX 0x00
enum ETATS{ SUIVRE_LIGNE, Y, INTERSECTION, TAPE, MORE_TAPE, TUNNEL, CARRE, COIN, CHECKPOINT, MUSIQUE, FIN };

void initialisationMaterielle();
