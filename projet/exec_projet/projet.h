#include "tp.h"
#include "memoire_24.h"
#include "usart.cpp"

void initialiser();
uint8_t getEtatSensor();
uint8_t getPin(uint8_t pin, uint8_t numero);
uint8_t getGauche(uint8_t pin);
uint8_t getDroit(uint8_t pin);
