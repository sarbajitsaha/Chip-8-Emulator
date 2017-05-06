//
// Created by sarbajit on 5/5/17.
//

#include <cstring>
#include <fstream>
#include <iostream>
#include "chip8.h"

//constructor
Chip8::Chip8()
{
    unsigned char chip8_fontset[80] =
            {
                    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                    0x20, 0x60, 0x20, 0x20, 0x70, // 1
                    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
            };

    pc = 0x200; // 0x000 to 0x1FF is reserved for  interpreter

    //resetting registers
    I = 0;
    sp = 0;
    sound_timer = 0;
    delay_timer = 0;

    //clear registers, stack and memory
    memset(V, 0, sizeof(V));
    memset(stack, 0, sizeof(stack));
    memset(memory, 0, sizeof(memory));

    draw_flag = false;

    //load fontset from 0 to 80
    for (int i = 0; i < 80; i++)
    {
        memory[i] = chip8_fontset[i];
    }

    //resetting display and keypad
    memset(display, 0, sizeof(display));
    memset(keypad, 0, sizeof(keypad));
}

//function to load ROM, with path to ROM given as argument
bool Chip8::load_rom(std::string rom_path)
{
    std::ifstream f(rom_path, std::ios::binary | std::ios::in);
    if (!f.is_open())
    {
        return false;
    }
    //load in memory from 0x200(512) onwards
    char c;
    int j = 512;
    for (int i = 0x200; f.get(c); i++)
    {
        if (j >= 4096)
        {
            return false; //file size too big memory space over so exit
        }
        memory[i] = (uint8_t) c;
        j++;
    }
    return true;
}


bool Chip8::get_draw_flag()
{
    return draw_flag;
}

void Chip8::set_draw_flag(bool flag)
{
    draw_flag = flag;
}

int Chip8::get_display_value(int i)
{
    return display[i];
}

void Chip8::set_keypad_value(int index, int val)
{
    keypad[index] = val;
}

//emulates one cycle
void Chip8::single_cycle(bool trace_mode, bool sound_on)
{
    //2 byte opcode
    int opcode = (memory[pc] << 8) | (memory[pc + 1]);
    int opcode_msb_nibble = get_nibble(opcode, 12, 0xF000); //if value is ABCD(each 4 bits), it returns A
    int val, reg, reg1, reg2;

    if (trace_mode)
    {
        printf("%.4X %.4X %.2X ", pc, opcode, sp);
        for (int i = 0; i < 15; i++)
        {
            printf("%.2X ", V[i]);
        }
        printf("\n");
    }

    switch (opcode_msb_nibble)
    {
        case 0:
            //only found 00E0 and 00EE starting with 0 so check for these two
            switch (opcode)
            {
                case 0x00E0:
                    memset(display, 0, sizeof(display));
                    draw_flag = true;
                    pc += 2;
                    break;

                case 0x00EE:
                    sp--;
                    pc = stack[sp];
                    pc += 2;
                    break;

                default:
                    //incorrect opcode
                    std::cerr << "Unknown opcode- " << opcode;
                    break;
            }
            break;

        case 1:
            pc = opcode & 0x0FFF;
            break;

        case 2:
            //only instruction is 2NNN Calls subroutine at NNN.
            //so put current address in stack and move pc to NNN
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
            break;

        case 3:
            //only instruction is 3XNN Skips the next instruction if VX equals NN.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            pc += 2; //next instruction
            if (V[reg] == val)
            {
                pc += 2; //adding 2 again so this instruction is skipped
            }
            break;

        case 4:
            //instruction is 4XNN. Skips the next instruction if VX doesn't equal NN.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            pc += 2; //next instruction
            if (V[reg] != val)
            {
                pc += 2; //adding 2 again so this instruction is skipped
            }
            break;

        case 5:
            //instruction is 5XY0. Skips the next instruction if VX equals VY.
            reg1 = get_nibble(opcode, 8, 0x0F00);
            reg2 = get_nibble(opcode, 4, 0x00F0);
            pc += 2;
            if (V[reg1] == V[reg2])
            {
                pc += 2;
            }
            break;

        case 6:
            //instruction is 6XNN. Sets VX to NN.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            V[reg] = val;
            pc += 2;
            break;

        case 7:
            //instruction is 7XNN. Adds NN to VX.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            V[reg] += val;
            pc += 2;
            break;

        case 8:
            //multiple instructions possible
            val = get_nibble(opcode, 0, 0x000F); //extract last 4 bits
            switch (val)
            {
                case 0:
                    //8XY0. Sets VX to the value of VY.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    V[reg1] = V[reg2];
                    pc += 2;
                    break;

                case 1:
                    //8XY1. Sets VX to VX or VY. (Bitwise OR operation) VF is reset to 0.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    V[reg1] |= V[reg2];
                    V[0xF] = 0;
                    pc += 2;
                    break;

                case 2:
                    //8XY2. Sets VX to VX and VY. (Bitwise AND operation) VF is reset to 0.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    V[reg1] &= V[reg2];
                    V[0xF] = 0;
                    pc += 2;
                    break;

                case 3:
                    //8XY3. Sets VX to VX xor VY. VF to 0
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    V[reg1] ^= V[reg2];
                    V[0xF] = 0;
                    pc += 2;
                    break;

                case 4:
                    //Adds VY to VX. VF is set to 1 when there's a carry, and to 0 when there isn't.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    if (V[reg1] + V[reg2] > 0xFF)
                    {
                        V[0xF] = 1;
                    }
                    else
                    {
                        V[0xF] = 0;
                    }
                    V[reg1] += V[reg2];
                    V[reg1] = (int8_t) V[reg1];
                    pc += 2;
                    break;

                case 5:
                    //VY is subtracted from VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    if (V[reg1] < V[reg2])
                    {
                        V[0xF] = 0;
                    }
                    else
                    {
                        V[0xF] = 1;
                    }
                    V[reg1] = (uint8_t) V[reg1] - (uint8_t) V[reg2];
                    pc += 2;
                    break;

                case 6:
                    //Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    V[0xF] = V[reg] & 0x1;
                    V[reg] >>= 1;
                    V[reg] = (uint8_t) V[reg];
                    pc += 2;
                    break;

                case 7:
                    //Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
                    reg1 = get_nibble(opcode, 8, 0x0F00);
                    reg2 = get_nibble(opcode, 4, 0x00F0);
                    if (V[reg1] > V[reg2])
                    {
                        V[0xF] = 0;
                    }
                    else
                    {
                        V[0xF] = 1;
                    }
                    V[reg1] = (uint8_t) V[reg2] - (uint8_t) V[reg1];
                    pc += 2;
                    break;

                case 0xE:
                    //Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift.[2]
                    reg = get_nibble(opcode, 8, 0x0F00);
                    V[0xF] = V[reg] >> 7;
                    V[0xF] = (uint8_t) V[0xF];
                    V[reg] <<= 1;
                    V[reg] = (uint8_t) V[reg];
                    pc += 2;
                    break;

                default:
                    std::cerr << "Unknown opcode- " << opcode;
                    break;
            }
            break;

        case 9:
            //9XY0. 	Skips the next instruction if VX doesn't equal VY.
            reg1 = get_nibble(opcode, 8, 0x0F00);
            reg2 = get_nibble(opcode, 4, 0x00F0);
            pc += 2;
            if (V[reg1] != V[reg2])
            {
                pc += 2;
            }
            break;

        case 10: //0xA
            //ANNN Sets I to the address NNN.
            I = opcode & 0x0FFF;
            pc += 2;
            break;

        case 11: //0xB
            //BNNN. Jumps to the address NNN plus V0.
            pc = (opcode & 0x0FFF);
            pc += V[0];
            break;

        case 12: //0xC
            //CXNN. Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.
            val = get_nibble(opcode, 0, 0x00FF); //extract the lower 8 bits
            reg = get_nibble(opcode, 8, 0x0F00); //get the X from opcode
            V[reg] = 5/*(rand() % (0xFF + 1))*/ & val;
            pc += 2;
            break;

        case 13: //0xD
        {
            //DXYN. Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
            // Each row of 8 pixels is read as bit-coded starting from memory location I;
            // I value doesn’t change after the execution of this instruction.
            // VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that doesn’t happen

            reg1 = get_nibble(opcode, 8, 0x0F00);
            reg2 = get_nibble(opcode, 4, 0x00F0);
            int x = V[reg1], y = V[reg2];
            int ht = opcode & 0x000F; //N
            int wt = 8;
            V[0x0F] = 0;

            for (int i = 0; i < ht; i++)
            {
                int pixel = memory[I + i];
                for (int j = 0; j < wt; j++)
                {
                    if ((pixel & (0x80 >> j)) != 0)
                    {
                        int index = ((x + j) + ((y + i) * 64)) % 2048;
                        if (display[index] == 1)
                        {
                            V[0x0F] = 1;
                        }
                        display[index] ^= 1;
                    }
                }
            }

            draw_flag = true;
            pc += 2;
            break;
        }
        case 14: //0x0E
            //two instructions possible
            val = get_nibble(opcode, 0, 0x00FF);
            switch (val)
            {
                case 0x9E:
                    //Skips the next instruction if the key stored in VX is pressed.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    pc += 2;
                    if (keypad[V[reg]] != 0)
                    {
                        pc += 2;
                    }
                    break;

                case 0xA1:
                    //Skips the next instruction if the key stored in VX isn't pressed.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    pc += 2;
                    if (keypad[V[reg]] == 0)
                    {
                        pc += 2;
                    }
                    break;

                default:
                    std::cerr << "Invalid opcode-> " << std::hex << opcode;
                    break;

            }
            break;

        case 15: //0x0F
            //multiple instructions possible
            val = get_nibble(opcode, 0, 0x00FF);
            switch (val)
            {
                case 0x07:
                    //FX07. Sets VX to the value of the delay timer.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    V[reg] = delay_timer;
                    pc += 2;
                    break;

                case 0x0A:
                    //FX0A. A key press is awaited, and then stored in VX.
                {
                    bool key_pressed = false;
                    reg = get_nibble(opcode, 8, 0x0F00);
                    for (int i = 0; i < 16; i++)
                    {
                        if (keypad[i] != 0)
                        {
                            key_pressed = true;
                            V[reg] = (uint8_t) i;
                        }
                    }
                    if (key_pressed)
                    {
                        pc += 2;
                    }
                    break;
                }

                case 0x15:
                    //FX15. Sets the delay timer to VX.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    delay_timer = V[reg];
                    pc += 2;
                    break;

                case 0x18:
                    //sets the sound timer to VX
                    reg = get_nibble(opcode, 8, 0x0F00);
                    sound_timer = V[reg];
                    pc += 2;
                    break;

                case 0x1E:
                    //Adds VX to I
                    reg = get_nibble(opcode, 8, 0x0F00);
                    if (I + V[reg] > 0xFFF)
                    {
                        V[0xF] = 1;
                    }
                    else
                    {
                        V[0xF] = 0;
                    }
                    I += V[reg];
                    I = (uint16_t) I;
                    pc += 2;
                    break;

                case 0x29:
                    //Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
                    reg = get_nibble(opcode, 8, 0x0F00);
                    I = V[reg] * 0x5;
                    pc += 2;
                    break;

                case 0x33:
                    //Stores the binary-coded decimal representation of VX, with the most significant of three digits at the address in I, the middle digit at I plus 1, and the least significant digit at I plus 2. (In other words, take the decimal representation of VX, place the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.)
                    reg = get_nibble(opcode, 8, 0x0F00);
                    memory[I] = (uint8_t) ((uint8_t) V[reg] / 100);
                    memory[I + 1] = (uint8_t) ((uint8_t) (V[reg] / 10) % 10);
                    memory[I + 2] = (uint8_t) ((uint8_t) (V[reg] % 100) % 10);
                    pc += 2;
                    break;

                case 0x55:
                    //Stores V0 to VX (including VX) in memory starting at address I
                    reg = get_nibble(opcode, 8, 0x0F00);
                    for (int i = 0; i <= reg; i++)
                    {
                        memory[I + i] = V[i];
                    }
                    I = I + reg + 1;
                    I = (uint16_t) I;
                    pc += 2;
                    break;

                case 0x65:
                    //Fills V0 to VX (including VX) with values from memory starting at address I
                    reg = get_nibble(opcode, 8, 0x0F00);
                    for (int i = 0; i <= reg; i++)
                    {
                        V[i] = memory[I + i];
                    }
                    I = I + reg + 1;
                    I = (uint16_t) I;
                    pc += 2;
                    break;

                default:
                    std::cerr << "Invalid opcode -> " << std::hex << opcode << std::endl;
                    break;
            }
            break;

        default:
            std::cerr << "Invalid opcode -> " << std::hex << opcode << std::endl;
            break;
    }

    if (delay_timer > 0)
    {
        delay_timer--;
    }
    if (sound_timer > 0)
    {
        if (sound_on)
        {
            printf("\a"); //terminal dependent, should be changed later
        }
        sound_timer--;
    }
}

//destructor
Chip8::~Chip8()
{}

//helper functions
int Chip8::get_nibble(int val, int bits, int val_to_binary_and = 0xFFFF) //extracts 4 bits from val
{
    return ((val & val_to_binary_and) >> bits);
}
