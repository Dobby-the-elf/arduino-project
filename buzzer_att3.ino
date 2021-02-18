/* Arduino tutorial - Buzzer / Piezo Speaker
   More info and circuit: http://www.ardumotive.com/how-to-use-a-buzzer-en.html
   Dev: Michalis Vasilakis // Date: 9/6/2015 // www.ardumotive.com */

const int buzzer = A0; //buzzer to arduino pin 9

void setup(){
  pinMode(A0, OUTPUT); // Set buzzer - pin 9 as an output
  Serial.begin(9600);
}

const int freq PROGMEM =3700;

void loop(){
  int voice_count=0;
  while(1){
    voice_count++;
    //freq+=100;
    tone(buzzer, freq); // Send 1KHz sound signal...
    delay(100);        // ...for X sec
    noTone(buzzer);     // Stop sound...
    delay(100);        // ...for X sec
    if(voice_count%3==0)delay(800);
  }
}
