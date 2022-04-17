#include <IRremote.h>

const int RECV_PIN = 7;
IRsend irsend(RECV_PIN);
//IRrecv irrecv(RECV_PIN);
//decode_results results;
unsigned long key_value = 0;

void setup() {
  irsend.sendNEC(0x20dfc03f, 32); // This command is for volume up

}

void loop() {
  irsend.sendNEC(0x20dfc03f, 32); // This command is for volume up

}
