#include "tp.h"
#include "memoire_24.h"
#include "usart.cpp"

#define PROGEN	0x80
#define	DEBUT	0x01
#define ATT 	0x02
#define DAL	0x44
#define DET	0x45
#define SGO	0x48
#define SAR	0x09
#define MAR	0x60
#define MAV	0x62
#define MRE	0x63
#define TRD	0x64
#define TRG	0x65
#define DBC	0xC0
#define FBC	0xC1
#define FIN	0xFF

void initialiser();
void routine();
void lireInstruction(uint8_t &instruction, uint8_t &operande);
void interpreterBytecode(const uint8_t &instruction, const uint8_t &operande);
void reprogrammation();
void execution();

void ATTf(const uint8_t &operande);
void DALf();
void DETf();
void SGOf(const uint8_t &operande);
void SARf();
void MARf();
void MAVf(const uint8_t &operande);
void MREf(const uint8_t &operande);
void TRDf();
void TRGf();
void DBCf(const uint8_t &operande);
void FBCf();
void FINf();
