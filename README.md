# Chip 8 Emulator
This is a simple Chip 8 emulator written in C++. CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for said computers.

[This article on Wikipedia](https://en.wikipedia.org/wiki/CHIP-8 "CHIP-8") explains the virtual machine(CPU,memory,display and keypad) and the opcodes supported. 

## Screenshots

### PONG2

![alt text](https://raw.githubusercontent.com/sarbajitsaha/Chip-8-Emulator/master/screenshots/pong2.jpg "PONG2 in Chip 8")

### BRIX

![alt text](https://raw.githubusercontent.com/sarbajitsaha/Chip-8-Emulator/master/screenshots/brix.jpg "BRIX in Chip 8")


## Building

Cmake and SDL2 are required for this project. You can install them on Ubuntu using 

```
$ sudo apt-get install cmake libsdl2-dev
```


For compilation 

```
$ cd cmake-build-debug
$ cmake ..
$ make
```

The executable file, Chip8_Emulator, will be in the cmake-build-debug folder

## Running

You need to provide the path to a ROM file as an argument. 23 ROMS have already been provided in the "roms" directory (downloaded from https://www.zophar.net/pdroms/chip8/chip-8-games-pack.html).

So you can start the PONG game available in the roms directory by

```
./Chip8_Emulator ../roms/PONG
```

## Test

To run tests

```
cd cmake-build-debug
make tests
./tests
```

Catch testing framework is used. More tests need to be added, so PRs are welcome. 


## Modes

There are two modes available - trace mode and single step mode.
You can also type _-help_ to check usage

- Trace mode - Type -t to print the program counter, register values and opcode executed each cycle. It follows this pattern: 

   pc opcode I sp V0 to VF
   
- Single step mode - Type -s flag to execute one instruction at a time waiting for you to press enter after each cycle.

Both modes can be used simultaneously. 

## Keypad

The original Chip 8 had a hexadecimal keypad (0 - 9 and A - F). The key mapping here is as follows - 

| Chip 8 Key | Keyboard Key |
| :--------: | :----------: |
| `1`        | `1`          |
| `2`        | `2`          |
| `3`        | `3`          |
| `4`        | `Q`          |
| `5`        | `W`          |
| `6`        | `E`          |
| `7`        | `A`          |
| `8`        | `S`          |
| `9`        | `D`          |
| `0`        | `X`          |
| `A`        | `Z`          |
| `B`        | `C`          |
| `C`        | `4`          |
| `D`        | `R`          |
| `E`        | `F`          |
| `F`        | `V`          |


## License

This project is licensed under the GPLv3+.
The file LICENSE includes the full license text.
