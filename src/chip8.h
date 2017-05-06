//
// Created by sarbajit on 5/5/17.
//

#ifndef CHIP8_CHIP8EMULATOR_H
#define CHIP8_CHIP8EMULATOR_H


#include <cstdint>
#include <string>

class Chip8
{
private:
    //CPU
    uint8_t V[16]; //16 8 bit registers Vx where x ranges from 0 to F
    uint16_t I, pc;
    uint8_t sp; //16 bit register I,16 bit program counter, 8 bit stack pointer
    uint16_t stack[16]; //16 16bit values can be stored in stack
    uint8_t delay_timer, sound_timer; //delay and sound 8 bit registers

    //MEMORY
    uint8_t memory[4096]; //4k RAM

    //DISPLAY
    int display[64 * 32]; //64*32 display

    //KEYPAD
    int keypad[16]; //hexadecimal keypad

    //flags
    bool draw_flag; //if true, need to draw

    //helper functions
    int get_nibble(int, int, int); //returns 4 bits from 1st argument
    // right shifting by second argument number of bits with optional third argument to & first


public:
    Chip8(); //constructor
    bool load_rom(std::string); //returns false if any error occurs while loading
    bool get_draw_flag();

    void set_draw_flag(bool);

    void single_cycle(bool, bool);

    int get_display_value(int);

    void set_keypad_value(int, int);

    ~Chip8(); //destructor
};


#endif //CHIP8_CHIP8EMULATOR_H
