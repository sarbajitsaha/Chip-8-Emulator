# Chip 8 Emulator
This is a simple Chip 8 emulator written in C++. CHIP-8 is an interpreted programming language, developed by Joseph Weisbecker. It was initially used on the COSMAC VIP and Telmac 1800 8-bit microcomputers in the mid-1970s. CHIP-8 programs are run on a CHIP-8 virtual machine. It was made to allow video games to be more easily programmed for said computers.

[This article on Wikipedia](https://en.wikipedia.org/wiki/CHIP-8 "CHIP-8") explains the virtual machine(CPU,memory,display and keypad) and the opcodes supported. 


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

## Keypad

The original Chip 8 had a hexadecimal keypad (0 - 9 and A - F). The key mapping here is as follows - 

| Chip 8 Key | Keyboard Key |
| :--------: | :----------: |
| `1`        | `1`          |
| `2`        | `2`          |
| `3`        | `3`          |
| `4`        | `4`          |
| `5`        | `Q`          |
| `6`        | `W`          |
| `7`        | `E`          |
| `8`        | `R`          |
| `9`        | `A`          |
| `0`        | `S`          |
| `A`        | `D`          |
| `B`        | `F`          |
| `C`        | `Z`          |
| `D`        | `X`          |
| `E`        | `C`          |
| `F`        | `V`          |

## License

This project is licensed under the GPLv3+.
The file LICENSE includes the full license text.
