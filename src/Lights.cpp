#include <Arduino.h>
#include "Settings.h"
#include "Map.h"

extern boolean flagState;
extern int blinkTime;
extern short int level;
extern short int sequence[];

/*Funzione di blink associata all'interrupt su Timer1*/
void blinky(){
  if (!flagState){
    digitalWrite(RED_LED, HIGH);  
  } else {
    digitalWrite(RED_LED, LOW);  
  }
  flagState = !flagState;
}

/*Funzione per fare un blink del led*/
void blinkLed(int ledPin){
  digitalWrite(ledPin, HIGH);
  delay(blinkTime);
  digitalWrite(ledPin, LOW);
  delay(blinkTime);
}

/*Funzione che simula una "danza" dei led per simulare il corretto conseguimento del livello*/
void ledDance(){
  digitalWrite(LED1, HIGH);
  delay(500);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, HIGH);
  delay(500);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, HIGH);
  delay(500);
  digitalWrite(LED3, LOW);
  digitalWrite(RED_LED, HIGH);
  delay(500);
  digitalWrite(RED_LED, LOW);
  delay(1000);
}

/*Funzione per accendere/spegnere tutti i led in una volta sola
  highLow può essere HIGH(true) or LOW(false)*/
void switchAll(boolean highLow){
  digitalWrite(LED1, highLow);
  digitalWrite(LED2, highLow);
  digitalWrite(LED3, highLow);
  digitalWrite(RED_LED, highLow);
}

/*Funzione per mostrare la sequenza, illuminando di mano in mano
  i rispettivi led*/
void showSequence(){
  for(int i=0; i<=level; i++){
    blinkLed(mapButtonToLed((mapNumberToButton(sequence[i]))));
  }
}
