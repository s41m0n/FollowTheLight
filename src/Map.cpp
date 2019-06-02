#include "Map.h"
#include "Settings.h"

/*Funzione per mappare ad ogni numero il bottone associato*/
int mapNumberToButton(int number){
    switch(number){
      case 1: {return BTN1;break;}
      case 2: {return BTN2;break;}
      case 3: {return BTN3;break;}  
    }
}

/*Funzione per mappare ad ogni bottone il led associato*/
int mapButtonToLed(int button){
    switch(button){
      case BTN1: {return LED1;break;};
      case BTN2: {return LED2;break;};
      case BTN3: {return LED3;break;};
    }
}

/*Funzione per mappare ad bottone il suo successivo
  (utile in quanto non vi è il vincolo di tenere i bottoni in 3 pin consecutivi)*/
int getNextButtonFromButton(int button){
    switch(button){
      case BTN1: {return BTN2;break;};
      case BTN2: {return BTN3;break;};
      case BTN3: {return BTN1;break;};
    }
}

