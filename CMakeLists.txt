cmake_minimum_required(VERSION 3.2)
project(Chip8_Emulator)

set(CMAKE_CXX_STANDARD 11)

INCLUDE(FindPkgConfig)
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
set(SOURCE_FILES src/main.cpp src/chip8.cpp src/chip8.h)
add_executable(Chip8_Emulator ${SOURCE_FILES})
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES})
