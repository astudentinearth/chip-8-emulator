cmake_minimum_required(VERSION 3.24)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

project(chip8 VERSION 1.0 LANGUAGES CXX)
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

add_library(chip8 STATIC chip8/chip8.cpp chip8/emulator.cpp chip8/util.cpp)

add_executable(chip8-emulator 
  src/main.cpp 
  src/App.cpp 
  src/EmulatorRenderer.cpp 
  src/EmulatorWindow.cpp
  src/EmulatorToolbar.cpp
  src/EmulatorSource.cpp
  src/EmulatorDebugger.cpp
  src/VCenterBox.cpp
  src/Buzzer.cpp
  resources.qrc
)
target_include_directories(chip8 PUBLIC include/)
target_link_libraries(chip8-emulator PRIVATE chip8 Qt6::Widgets Qt6::Multimedia)

