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
  uint8_t v[16]; //V0, V1, V2, V3, V4, V5, V6, V7, V8, V9 ,VA, VB, VC, VD, VE, VF; general purpose variable registers 
  // VF is general purpose but also used as a flag register
  //the memory
  uint8_t RAM[4096]; //putting here for the moment
  void run(){
    //main running loop
    if (refreshTimer){
      updateTimers(delayTimer);
      updateTimers(soundTimer);
    }
  }
  uint16_t fetch(){
    //read instructions from memory - 2 bytes
    //combine in to 16-bit value
    uint16_t instruction = RAM[pc] << 8;
    instruction = instruction || RAM[pc + 1];
    pc +=2;
    return instruction;
  }
  void execute(uint16_t fetchedValue){
        //each of the nibbles mean different things
    uint16_t addr = fetchedValue & 0xC; //we want lowest 12 bits only
    uint8_t n = fetchedValue & 0xF; //lowest 4 bits
    uint8_t x = (fetchedValue & 0xF00) >> 8; //lowest 4 bits of high byte
    uint8_t y = (fetchedValue & 0x00F0) >> 4; //highest 4 bits of low byte
    uint8_t kk = fetchedValue & 0xFF; //lowest 8 bits of instruction
    switch(fetchedValue & 0xF000){ //not sure about this. Used https://www.freecodecamp.org/news/creating-your-very-own-chip-8-emulator/
      case 0x0000:
        switch (fetchedValue){
          case 0x00E0: //CLS
            //clears screen
            break;
          case 0x00EE: //RET
            //set program counter to value at top of stack then decrement stack pointer
            pc = stack[sp];
            sp--;
            break;
        }
      case 0x1000: //JP addr
        //set program counter to the value of lowest 12 bits of instruction (addr)
        pc = addr;
        break;
      case 0x2000: //call addr
        //increment stack pointer, put current program counter on top of stack. PC set to addr
        sp ++;
        stack[sp] = pc;
        pc = addr;
        break;
      case 0x3000: //SE Vx, byte - Skip next instruction if Vx = kk
        //if value in register x = kk then increment program counter
        if (v[x] == kk){
          pc += 2;
        }
        break;
      case 0x4000: //SNE Vx, byte - Skip next instruction if Vx != kk
        //if value in register x != kk then increment program counter
        if (v[x] != kk){
          pc += 2;
        }
        break;
      case 0x5000: //SE Vx, Vy - Skip next instruction if Vx = Vy
        //if value in register x = value in register y increment program counter
        if (v[x] == v[y]){
          pc += 2;
        }
        break;
      case 0x6000: //LD Vx, byte - Set Vx = kk
        v[x] = kk;
        break;
      case 0x7000: //7xkk - ADD Vx, byte Set Vx = Vx + kk
        v[x] = v[x] + kk;
        break;
      case 0x8000:
        switch(n){ //we decide based on lowest 4 bits
          case 0x0: //LD Vx, Vy - Set Vx = Vy
            v[x] = v[y];
            break;
          case 0x1: //OR Vx, Vy - Set Vx = Vx OR Vy
            v[x] = v[x] | v[y];
            break;
          case 0x2: //AND Vx, Vy - Set Vx = Vx AND Vy
            v[x] = v[x] & v[y];
            break;
          case 0x3: //XOR Vx, Vy - Set Vx = Vx XOR Vy
            v[x] = v[x] ^ v[y];
            break;
          case 0x4: //ADD Vx, Vy - Set Vx = Vx + Vy, set VF = carry
            break;
          case 0x5: //SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow
            break;
          case 0x6: //SHR Vx {, Vy} - Set Vx = Vx SHR 1
            break;
          case 0x7: //SUBN Vx, Vy - Set Vx = Vy - Vx, set VF = NOT borrow.
            break;
          case 0xE: //SHL Vx {, Vy - Set Vx = Vx SHL 1
            break;
        }
        break;
      case 0x9000: //SNE Vx, Vy - Skip next instruction if Vx != Vy
        if (v[x] != v[y]){
          pc += 2;
        }
        break;
      case 0xA000: //LD I, addr - Set I = nnn
        I = addr;
        break;
      case 0xB000: //JP V0, addr - Jump to location nnn + V0
        pc = addr + v[0];
        break;
      case 0xC000: //RND Vx, byte - Set Vx = random byte AND kk
        break;
      case 0xD000: //DRW Vx, Vy, nibble - Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        break;
      case 0xE000:
        switch(kk){ //decide based on lowest byte
          case 0x9E: //SKP Vx - Skip next instruction if key with the value of Vx is pressed
            break;
          case 0x91: //SKNP Vx - Skip next instruction if key with the value of Vx is not pressed
            break;
        }
        break;
      case 0xF000:
        switch (kk) {
          case 0x07: //LD Vx, DT - Set Vx = delay timer value
            v[x] = delayTimer;
            break;
          case 0x0A: //LD Vx, K - Wait for a key press, store the value of the key in Vx
            break;
          case 0x15: //LD DT, Vx - Set delay timer = Vx
            delayTimer = v[x];
            break;
          case 0x18: //LD ST, Vx - Set sound timer = Vx
            soundTimer = v[x];
            break;
          case 0x1E: //ADD I, Vx - Set I = I + Vx
            I = I + v[x];
            break;
          case 0x29: //LD F, Vx - Set I = location of sprite for digit Vx
            break;
          case 0x33: //LD B, Vx - Store BCD representation of Vx in memory locations I, I+1, and I+2
            break;
          case 0x55: //LD [I], Vx - Store registers V0 through Vx in memory starting at location I
            break;
          case 0x65: //LD Vx, [I] - Read registers V0 through Vx from memory starting at location I
            break;
        }
        break;
    }
  }
  void push(uint8_t &value){
    //to push on to stack
    //not sure if below correct, my first guess
    stack[sp] = value;
    sp ++;
  }
  uint8_t pop(){
    //to pop from the stack
    uint8_t returnValue = stack[sp];
    stack[sp] = 0;
    sp --;
    return returnValue;
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