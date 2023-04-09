#include <Arduino.h>
/* chip8 emulator using Arduino
http://devernay.free.fr/hacks/chip8/C8TECH10.HTM#memmap is the reference used
https://tobiasvl.github.io/blog/write-a-chip-8-emulator/ for guidance
*/

class CPU {
  public :
  uint8_t sp; //stack pointser
  uint16_t pc; //program counter
  uint16_t I; //index register
  uint8_t stack[64];//add stack
  uint8_t delayTimer; //decremented at 60hz until it reaches 0. 16.67ms between updates
  uint8_t soundTimer; //functions the same as delay timers but beeps always unless 0
  uint16_t cycleCounter; //
  float refreshRate = 16.67;
  uint8_t V0, V1, V2, V3, V4, V5, V6, V7, V8, V9 ,VA, VB, VC, VD, VE; //general purpose variable registers 
  uint8_t VF; //general purpose but also used as a flag register
  //the memory
  uint8_t RAM[4096]; //putting here for the moment
  void run(){
    //main running loop
  }
  uint16_t fetch(){
    //read instructions from memory - 2 bytes
    //combine in to 16-bit value
    uint16_t instruction = RAM[pc] << 8;
    instruction = instruction || RAM[pc + 1];
    pc +=2;

    return instruction;
  }
  void decode(uint16_t fetchedValue){
    //each of the nibbles mean different things
    uint16_t addr = fetchedValue & 0xC; //we want lowest 12 bits only
    uint8_t n = fetchedValue & 0xF; //lowest 4 bits
    uint8_t x = (fetchedValue & 0xF00) >> 8; //lowest 4 bits of high byte
    uint8_t y = (fetchedValue & 0xF0) >> 4; //highest 4 bits of low byte
    uint8_t k = fetchedValue & 0xFF; //lowest 8 bits of instruction

  }
  void execute(){
    if (refreshTimer){
      updateTimers(delayTimer);
      updateTimers(soundTimer);
    }

  }
  void push(uint8_t &value){
    //to push on to stack
    //not sure if below correct, my first guess
    stack[sp] = value;
    sp ++;
  }
  void pop(){
    //to pop from the stack
    stack[sp] = 0;
    sp --;
  }
  bool refreshTimer(){
    // check if we're
    if (millis() - cycleCounter >= refreshRate){
      cycleCounter = 0;
      return true;
    }
    else {
      return false;
    }
  }
  void updateTimers(uint8_t &tValue){
    if (tValue > 0){
      tValue --;
    }
  }
};


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}