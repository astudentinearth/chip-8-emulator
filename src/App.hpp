
#pragma once
#include "Color.hpp"
#include "chip8.hpp"
#include <future>
#include <memory>
#include <string>

struct AppContext {
    std::string filename;
    ColorScheme colorscheme = ColorScheme::Default;
    std::unique_ptr<chip8::Chip8Emulator> emulator;
    std::promise<void> window_ready;
    int argc;
    const char** argv;
    std::string getWindowTitle() { return "CHIP-8 Emulator ("s + filename  + ")"s; }
    bool debuggerVisible{true};
};

int runQt6App(AppContext& context);


