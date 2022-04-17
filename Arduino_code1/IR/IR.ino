#include <IRremote.h>

int led = 13;
const int RECV_PIN = 7;
IRrecv irrecv(RECV_PIN);
decode_results results;
int count = 0;

void setup(){
  Serial.begin(57600);
  irrecv.enableIRIn();
  irrecv.blink13(true);
  pinMode(led, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop(){
  if(count == 0){
    digitalWrite(led, HIGH);
  }
  if (irrecv.decode(&results)){
        Serial.println(results.value, HEX);
        irrecv.resume();
        digitalWrite(led, LOW);
        //if(results.value){
          Serial.print("Can be done \n");
          digitalWrite(led, HIGH);
          count = 1;
        //}
  }
  
}
