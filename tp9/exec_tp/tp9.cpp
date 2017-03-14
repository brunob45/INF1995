/************************* BRANCHEMENTS **************************
 *								 *
 *	ROUES	ENABLE -> PORTD 5,6	DIRECTION -> PORTD 3,4	 *
 *	PIEZO	PORTD7,8					 *
 *	DEL	(+) -> PORTA1		(-) -> PORTA2		 *
 * 	ProgEn	VCC -> PINA8					 *
 *****************************************************************/

#include "tp9.h"

volatile uint8_t compteur, boucleMax;
volatile uint16_t index = 0x00, indexBoucle;

int main()
{
	initialiser();
	routine();
	
	if(PINA & PROGEN)	    // Si le bouton pressoir est pesé, 
		reprogrammation();  //le robot sera en mode reprogrammation
	else
		execution();
}

void initialiser() {
	DDRA = 0x7f;	//PINA8 en ENTREE, les autres en SORTIE.
	DDRB = SORTIE;
	DDRC = SORTIE;
	DDRD = SORTIE;
	
	//CTC Piezo
	TCCR2A = _BV(WGM21) | _BV(COM2A0);
	TCCR2B = _BV(CS02); 
	OCR2A = 0;

	 //PWM Roues
	TCCR1A = _BV(WGM10) | _BV(COM1A1) | _BV(COM1B1);
	TCCR1B = _BV(CS11) ; 
	TCCR1C = 0;
	
	initialisationUART();
}

void routine() {
	fadeDEL(ROUGE, VERT);
	PORTA = ETEINT;
	_delay_ms(500);
	PORTA = VERT;
	_delay_ms(500);
	PORTA = ETEINT;
}

void reprogrammation()
{
	uint8_t data = 0x00;
	index = 0x00;
	
	PORTA = ROUGE;
	
	do
	{
		data = receptionUART();
		Memoire24CXXX().ecriture(index++, data);
	} while(data != FIN) ;
	
	while(PINA & PROGEN);
	routine();
}

void execution() {
	uint8_t nbInstructions, instruction, operande;
		
	Memoire24CXXX().lecture(index, &nbInstructions, 2);
	index += 2;

	do
	{
		lireInstruction(instruction, operande);
		transmissionUART(instruction); // Pour le debugging.
	} while (instruction != DEBUT) ;

	do
	{
		lireInstruction(instruction, operande);
		transmissionUART(instruction); // Pour le debugging.
		
		interpreterBytecode(instruction, operande);
	} while (instruction != FIN) ;
}

void lireInstruction(uint8_t &instruction, uint8_t &operande) {
	Memoire24CXXX().lecture(index++, &instruction);
	Memoire24CXXX().lecture(index++, &operande);
}

void interpreterBytecode(const uint8_t &instruction, const uint8_t &operande) {
	switch(instruction)
	{
		case ATT: ATTf(operande); break;
		case DAL: DALf(); break;
		case DET: DETf(); break;
		case SGO: SGOf(operande); break;
		case SAR: SARf(); break;
		case MAR: MARf(); break;
		case MAR + 1: MARf(); break;
		case MAV: MAVf(operande); break;
		case MRE: MREf(operande); break;
		case TRD: TRDf(); break;
		case TRG: TRGf(); break;
		case DBC: DBCf(operande); break;
		case FBC: FBCf(); break;
		case FIN: FINf();
		default: break;
	}
}

void ATTf(const uint8_t &operande) { 
	for(uint8_t cpt = 0; cpt < operande; cpt++)
		_delay_ms(25);
}
void DALf() { PORTA = ROUGE; }
void DETf() { PORTA = ETEINT; }
void SGOf(const uint8_t &operande) { OCR2A = 1911 * pow(2.7, -0.058 * operande); }
			//1981 * pow(2.7, 0.058 * operande); }//vive excel 1981
void SARf() { OCR2A = 0; }
void MARf() { ajustementRoues(0,0, AVANCER); }
void MAVf(const uint8_t &operande) { 
	ajustementRoues(0xff, 0xff, AVANCER);
	_delay_ms(50);
	ajustementRoues(operande, operande, AVANCER); }
void MREf(const uint8_t &operande) { 
	ajustementRoues(0xff, 0xff, RECULER);
	_delay_ms(50);
	ajustementRoues(operande, operande, RECULER); }
void TRDf() { 
	ajustementRoues(0xff, ETEINT, AVANCER);
	_delay_ms(900);
	ajustementRoues(ETEINT, ETEINT, AVANCER); 
}
void TRGf() { 
	ajustementRoues(ETEINT, 0xff, AVANCER); 
	_delay_ms(800);
	ajustementRoues(ETEINT, ETEINT, AVANCER); 	
}
void DBCf(const uint8_t &operande) { 
	indexBoucle = index;
	compteur = operande;
}
void FBCf() { 
	if(compteur-- > 0)
		index = indexBoucle; 
}
void FINf()
{
	DETf(); SARf(); MARf();
}
