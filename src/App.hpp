
#pragma once
#include "Color.hpp"
#include "chip8.hpp"
#include <SDL3/SDL.h>
#include <future>
#include <memory>
#include <string>


struct AppContext {
    std::string filename;
    ColorScheme colorscheme = ColorScheme::Default;
    std::shared_ptr<chip8::Chip8Emulator> emulator;
    chip8::Framebuffer& framebuffer;
    std::promise<void> window_ready;
    int argc;
    const char** argv;
    std::string getWindowTitle() { return "CHIP-8 Emulator ("s + filename  + ")"s; }
    bool debuggerVisible{true};
};

void runSDLApp(AppContext& context);
int runQt6App(AppContext& context);


