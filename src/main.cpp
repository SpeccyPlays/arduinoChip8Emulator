#include <Arduino.h>

class CPU {
  uint16_t PC; //program counter
  uint16_t I; //index register
  //add stack
  uint8_t delayTimer; //decremented at 60hz until it reaches 0
  uint8_t soundTimer;

};
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}