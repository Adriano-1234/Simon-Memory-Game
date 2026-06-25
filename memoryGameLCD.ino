#include <LiquidCrystal.h>
#include <EEPROM.h>

LiquidCrystal lcd(13, 12, 11, A1, A2, A3);

int ledPins[] = {2, 3, 4, 5};
int buttonPins[] = {6, 7, 8, 9};
int buzzerPin = 10;
int tones[] = {262, 330, 392, 523};
int sequence[100];
int sequenceLength = 0;
int HighScore = 0;

void lcdLoop(){
  //prints round number
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Round: ");
  lcd.print(sequenceLength);

  //prints high score
  lcd.setCursor(0,1);
  lcd.print("High Score: ");
  lcd.print(HighScore);
}

int getButtonPressed(){
  for(int i = 0; i < 4; i++){
    //if button is pressed then returns its index
    if(digitalRead(buttonPins[i]) == LOW){
      // wait for release of button
      while(digitalRead(buttonPins[i]) == LOW){}

      return i;
    }
  }
  return -1;
}

void gameOver(){
  //prints gameover on screen
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("GAME OVER");

  //lights flash when you lose
  for(int i = 0; i < 4; i++){
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[1], HIGH);
    digitalWrite(ledPins[2], HIGH);
    digitalWrite(ledPins[3], HIGH);
    tone(buzzerPin, 131);
    delay(100);
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[1], LOW);
    digitalWrite(ledPins[2], LOW);
    digitalWrite(ledPins[3], LOW);
    noTone(buzzerPin);
    delay(100);
  }
  
  //finds the high score
  if(sequenceLength > HighScore){
    HighScore = sequenceLength;
    EEPROM.put(0, HighScore);
  }

  //restarts game
  sequenceLength = 0;
}

void setup() {

  //adds memory location of high score
  EEPROM.get(0, HighScore);

  //safety check
  if (HighScore < 0 || HighScore > 100) {
    HighScore = 0;
  }
    
  //creates random sequence everytime
  randomSeed(analogRead(A0));

  //initialize led pins and button pins
  for(int i = 0; i < 4; i++){
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  //initialize number of rows/columns in display
  lcd.begin(16, 2);
}

void loop() {
  //initialize sequence every round
  sequence[sequenceLength] = random(0, 4);
  sequenceLength++;

  lcdLoop();

  //show sequence
  for(int i = 0; i < sequenceLength; i++){

    digitalWrite(ledPins[sequence[i]], HIGH);
    tone(buzzerPin, tones[sequence[i]]);
    delay(300);
    digitalWrite(ledPins[sequence[i]], LOW);
    noTone(buzzerPin);
    delay(300);

  }

  for(int j = 0; j < sequenceLength; j++){

    int input;

    //waits for button to be pressed
    do{
      input = getButtonPressed();
    }while(input == -1);

    //if the button and sequnece index are the same then keep going
    if(input == sequence[j]){
      digitalWrite(ledPins[input], HIGH);
      tone(buzzerPin, tones[input]);
      delay(300);
      digitalWrite(ledPins[input], LOW);
      noTone(buzzerPin);
      delay(300);
    }

    //end game if player messes up
    else{
      gameOver();
    }

  }


}
