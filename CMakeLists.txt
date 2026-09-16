cmake_minimum_required(VERSION 3.24)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(chip8)

option(CHIP8_USE_VENDORED_SDL "Build bundled SDL" ON)
  if(CHIP8_USE_VENDORED_SDL)
      add_subdirectory(vendor/SDL EXCLUDE_FROM_ALL)
  else()
      find_package(SDL3 CONFIG REQUIRED)
  endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
add_library(chip8 STATIC chip8/chip8.cpp chip8/emulator.cpp chip8/util.cpp)
add_executable(chip8-emulator src/main.cpp src/Window.cpp src/App.cpp)
target_include_directories(chip8 PUBLIC include/)
target_link_libraries(chip8-emulator PRIVATE chip8 SDL3::SDL3)

