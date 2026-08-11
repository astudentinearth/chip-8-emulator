cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(chip8)

add_library(chip8 STATIC chip8/chip8.cpp)
add_executable(chip8-emulator src/main.cpp)
target_include_directories(chip8 PUBLIC include/)
target_link_libraries(chip8-emulator PRIVATE chip8)

