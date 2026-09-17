
#pragma once
#include "Color.hpp"
#include "chip8.hpp"
#include <SDL3/SDL.h>
#include <memory>
#include <string>


struct AppContext {
    std::string filename;
    ColorScheme colorscheme = ColorScheme::Default;
    std::shared_ptr<chip8::Chip8Emulator> emulator;
    chip8::Framebuffer& framebuffer;
};

void runSDLApp(AppContext& context);
