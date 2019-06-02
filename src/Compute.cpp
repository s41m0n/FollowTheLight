#include <Arduino.h>
#include "Compute.h"

extern short int sequence[];
extern short int level;
extern short int difficulty;

/*Funzione che calcola il prossimo elemento della sequenza*/
void calculateSequence() {
  sequence[level] = random(1,4);  
}

/*Funzione che calcola il punteggio dell'utente*/
int calculateScore(){
  return  level*difficulty;
}
