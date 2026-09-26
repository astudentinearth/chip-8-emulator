
#include "App.hpp"
#include "Color.hpp"
#include "Window.hpp"
#include "chip8.hpp"
#include <SDL3/SDL.h>
#include <string>

using namespace std;
using namespace chip8;

void runSDLApp(AppContext &context) {
  SDL_Init(SDL_INIT_VIDEO);
  auto window = new EmulatorWindow(&context.framebuffer);
  window->setTitle("CHIP-8 Emulator");
  window->setColorScheme(context.colorscheme);
  context.window_ready.set_value();
  bool running = true;
  constexpr uint8_t KEY_IGNORE = 99;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
        context.emulator->hlt();
        break;
      }
      if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
        uint8_t key = KEY_IGNORE;
        if (event.key.key == SDLK_Q) {
          key = KEY_IGNORE;
          context.emulator->dumpState();
        }
        if (event.key.key >= SDLK_0 && event.key.key <= SDLK_9) {
          key = event.key.key - SDLK_0;
        }
        if (event.key.key >= SDLK_A && event.key.key <= SDLK_F) {
          key = event.key.key - SDLK_A;
        }
        if (key == KEY_IGNORE)
          continue;
        context.emulator->setKey(key, event.type == SDL_EVENT_KEY_DOWN ? true
                                                                       : false);
        if (context.emulator->getState() == EmulatorState::WaitingInput)
          context.emulator->continueWithKey(key);
      }
    }

    window->setDebugInfo(
        context.emulator->getReg(), context.emulator->getKeypad(),
        context.emulator->getState(), context.emulator->getDelayTimer(),
        context.emulator->getSoundTimer());
    window->draw();
  }
  delete window;
}
