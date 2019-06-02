#include "State.h"
#include "Settings.h"
#include <Arduino.h>


extern unsigned long lastInterrupt;
extern volatile short int state;

/*Funzione di interrupt per aumentare lo stato, segnale di inizio gioco*/
void increaseState(){
  unsigned long current = micros();
  if(current - lastInterrupt > BOUNCING_TIME){
    state++;
    lastInterrupt = current;
  }
}

/*Funzione di interrupt per decrementare lo stato, si ritorna alla regolazione potenziometro*/
void decreaseState(){
  unsigned long current = micros();
  if(current - lastInterrupt > BOUNCING_TIME){
    state--;
    lastInterrupt = current;
  }
}

/*Funzione di interrupt con BTN1 per svegliare Arduino e riniziare a giocare*/
void wakeUpToStart(){
  lastInterrupt = micros();  
}

