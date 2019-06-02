#include <Arduino.h>
#include "Evaluations.h"
#include "Settings.h"
#include "Map.h"
#include "Lights.h"

extern short int level;
extern short int state;
extern short int sequence[];

/*Attesa e valutazione input utente. Ritorna LOST se:
  1-Utente non finisce la sequenza entro il tempo(TIME_FOR_...*...)
  2-Utente sbaglia sequenza
  
  Alcuni raffinamenti come l'ignore dei possibili bouncing rendono questa funzione
  molto versatile.*/
int evaluateInput(){
    short int currentIndex = 0;
    unsigned long startTime = micros();
    unsigned long lastButtonClickedTime = 0;
    int currentButton = mapNumberToButton(sequence[currentIndex]);
    
    while(micros() - startTime <= TIME_FOR_INPUT_BUTTON*(level+1) && state == PLAYING && currentIndex <= level){
      if((micros() - lastButtonClickedTime > BOUNCING_TIME) ){
          if(digitalRead(currentButton) == HIGH 
              && digitalRead(getNextButtonFromButton(currentButton)) == LOW 
               && digitalRead(getNextButtonFromButton(getNextButtonFromButton(currentButton))) == LOW){
                lastButtonClickedTime = micros();
                switchAll(LOW);
                digitalWrite(mapButtonToLed(currentButton), HIGH);
                currentIndex++;  
                currentButton = mapNumberToButton(sequence[currentIndex]);
          }else if(digitalRead(getNextButtonFromButton(currentButton)) == HIGH 
              || digitalRead(getNextButtonFromButton(getNextButtonFromButton(currentButton))) == HIGH){
             return LOSE;
          }
      }
    }
    
    currentIndex--;
    return currentIndex==level? WIN : LOSE;
}

