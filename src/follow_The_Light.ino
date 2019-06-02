#include "TimerOne.h"
#include "Settings.h"
#include "Lights.h"
#include "Map.h"
#include "State.h"
#include "Evaluations.h"
#include "Compute.h"
#include <avr/sleep.h>

/*Dichiarazione e inizializzazione variabili globali. In ordine abbiamo:*/
/*Stato di gioco (Volatile per evitare preoblema presentato a lezione)*/
volatile short int state = 0;
/*Flag per interrupt blinky*/
boolean flagState = false;
/*Array rappresentante la sequenza*/
short int sequence[MAX_LEVEL]= {0};
/*Variabile per il livello*/
short int level = 0;
/*Variabile per la difficoltà*/
short int difficulty = 0;
/*Velocità blink led in sequenza*/
short int blinkTime = 0;
/*Variabile che si salva i microsecondi dell'ultimo interrupt*/
unsigned long lastInterrupt = 0;

/*Arduino funzione setup*/
void setup() {
  pinMode(RED_LED, OUTPUT);
  pinMode(LED1, OUTPUT);      
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BTN1, INPUT);
  pinMode(BTN2, INPUT);
  pinMode(BTN3, INPUT);
  Serial.begin(9600);
  Timer1.initialize(1000000);
  randomSeed(analogRead(RND_SEED));
}

/*Funzione per inizializzare le varie variabili d'ambiente e settare gli interrupt*/
void initWorkspace() {
  state = 0;
  level = 0;
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_enable();
  attachInterrupt(digitalPinToInterrupt(BTN1), increaseState, RISING);
  Timer1.attachInterrupt(blinky);
}

/*Funzione di accoglienza, stampa le regole del gioco e attende
  che l'utente finisca i settings*/
/*In tutto ciò, è attaccato un'altro interrupt sul BTN2 per dare la possibilità
  all'utente di annullare la scelta della difficoltà*/
/*Infine, ancora non vi è il concetto di stato di gioco(playing,win,lose)
  per questo la variabile state è utilizzata con valori 0,1,2
  senza alcun riferimento alle macro*/
void welcome(){
  Serial.println("Welcome to Follow The Light!");
  Serial.println("Rules:");
  Serial.println("1-Repeat the sequence shown by leds using buttons");
  Serial.println("2-Red led means it's your turn");

  while(state != 2){
    Serial.println("Use Potentiometer to set difficulty (Button1->Start)");
    /*Aspetta finchè non schiaccio il BTN1 per iniziare*/
    while(state == 0);
    difficulty = map(analogRead(POT_PIN),0,1023,1,5); 
    Serial.print("Starting with difficulty: ");
    Serial.print(difficulty);
    Serial.println(" ? (Button1->Yes Button2->No)");
    attachInterrupt(digitalPinToInterrupt(BTN2), decreaseState, RISING);
    while(state==1);
    detachInterrupt(digitalPinToInterrupt(BTN2));
  }
  blinkTime = 700 - difficulty*100; 
  detachInterrupt(digitalPinToInterrupt(BTN1));
  Timer1.detachInterrupt();
  digitalWrite(RED_LED, LOW);
}

/*Loop del gioco, ad ogni livello calcola l'elemento nuovo della sequenza e la mostra;
  in seguito controlla gli input dell'utente ed esegue operazioni di check della routine
  per verificare se è cambiato qualcosa dello Stato di gioco(ora state assume PLAYING, WIN or LOSE)*/
void gameLoop(){
  Serial.println("Be ready, good luck :D");
  delay(2000);
  while(level < MAX_LEVEL && state == PLAYING){
    calculateSequence();
    showSequence();
    digitalWrite(RED_LED, HIGH);
    if(evaluateInput() == WIN){
      level++;
      Serial.print("Level ");
      Serial.print(level);
      Serial.println(" passed, Excellent!");
    }else {
      state = LOSE;
      return;  
    }
    switchAll(LOW);
    ledDance();
  }
  
  if(level == MAX_LEVEL){
    state = WIN;  
  } else {
    state = LOSE;  
  }
}

/*Funzione di GameOver, visualizza punteggio e messaggio*/
void gameOver(){
  switchAll(HIGH);
  switch(state){
    case WIN: {Serial.print("YOU WIN! SCORE = ");break;};
    case LOSE: {Serial.print("YOU LOST! SCORE = ");break;};
  }  
  Serial.println(calculateScore());
}

/*Sleep in PowerDown di Arduino*/
void sayBye(){
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  attachInterrupt(digitalPinToInterrupt(BTN1), wakeUpToStart, RISING);
  sleep_enable();
  Serial.println("I go to sleep, thanks for playing! (Button1->Restart)");
  delay(2000);
  sleep_mode();
  sleep_disable();
  detachInterrupt(digitalPinToInterrupt(BTN1));
  switchAll(LOW);
}

/*Super loop*/
void loop() {
  initWorkspace();
  welcome();
  gameLoop();
  gameOver();
  sayBye();
}
