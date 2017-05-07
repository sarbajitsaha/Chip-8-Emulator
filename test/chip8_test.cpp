//
// Created by sarbajit on 8/5/17.
//

#define CATCH_CONFIG_MAIN
#include "../catch/catch.hpp"
#include "../src/chip8.h"

TEST_CASE("load_rom function")
{
    Chip8 chip8;
    REQUIRE(chip8.load_rom("invalid_path") == false); //testing invalid path
    REQUIRE(chip8.load_rom("../screenshots/brix.jpg") == false); //testing rom size bigger than memory
    REQUIRE(chip8.load_rom("../roms/PONG") == true); //testing normal case
}

