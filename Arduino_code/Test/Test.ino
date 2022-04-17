int RaspiInpin = 8;
int ledPin = 11;
int buttonState = 0;
#include <IRremote.h>
IRsend irsend;

void setup(){
  int buttonState = 0;
  pinMode(RaspiInpin, INPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);
  
}

void loop(){
  digitalWrite(ledPin,LOW);
  

buttonState = digitalRead(RaspiInpin);
Serial.print(RaspiInpin);
if (buttonState == HIGH) {
    // turn LED on:
    for (int i = 0; i < 2; i++) {
      digitalWrite(ledPin, LOW);
    irsend.sendSony(0xa90, 12);
    delay(40);
    }
  } else if (buttonState == 0) {
    // turn LED off:
    digitalWrite(ledPin, LOW);
    Serial.print("LOW");
  }

  }
