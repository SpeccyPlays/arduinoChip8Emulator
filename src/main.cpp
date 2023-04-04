#include <Arduino.h>

class CPU {
  public :
  uint8_t sp; //stack pointser
  uint16_t pc; //program counter
  uint16_t I; //index register
  uint8_t stack[64];//add stack
  uint8_t delayTimer; //decremented at 60hz until it reaches 0
  uint8_t soundTimer; //functions the same as delay timers but beeps always unless 0
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

    return instruction;//update this
  }
  void decode(uint16_t fetchedValue){
    //each of the nibbles mean different things
  }
  void execute(){

  }
  void push(uint8_t value){
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
};


void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}