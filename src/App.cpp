
#include "Window.hpp"
#include "chip8.hpp"
#include <SDL3/SDL.h>
#include <string>

using namespace std;
using namespace chip8;

void runSDLApp(shared_ptr<Chip8Emulator> emulator, Framebuffer &fb) {
  SDL_Init(SDL_INIT_VIDEO);
  auto window = new EmulatorWindow(&fb);
  window->setTitle("CHIP-8 Emulator");
  bool running = true;
  constexpr uint8_t KEY_IGNORE = 99;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
        emulator->hlt();
        break;
      }
      if (event.type == SDL_EVENT_KEY_DOWN || event.type == SDL_EVENT_KEY_UP) {
        uint8_t key = KEY_IGNORE;
        switch (event.key.key) {
        case SDLK_0:
          key = 0;
          break;
        case SDLK_1:
          key = 1;
          break;
        case SDLK_2:
          key = 2;
          break;
        case SDLK_3:
          key = 3;
          break;
        case SDLK_4:
          key = 4;
          break;
        case SDLK_5:
          key = 5;
          break;
        case SDLK_6:
          key = 6;
          break;
        case SDLK_7:
          key = 7;
          break;
        case SDLK_8:
          key = 8;
          break;
        case SDLK_9:
          key = 9;
          break;
        case SDLK_A:
          key = 0xA;
          break;
        case SDLK_B:
          key = 0xB;
          break;
        case SDLK_C:
          key = 0xC;
          break;
        case SDLK_D:
          key = 0xD;
          break;
        case SDLK_E:
          key = 0xE;
          break;
        case SDLK_F:
          key = 0xF;
          break;
        case SDLK_Q:
          key = KEY_IGNORE;
          emulator->dumpState();
          break;
        default:
          key = KEY_IGNORE;
          break;
        }
        if (key == KEY_IGNORE)
          continue;
        emulator->setKey(key, event.type == SDL_EVENT_KEY_DOWN ? true : false);
        if (emulator->getState() == EmulatorState::WaitingInput)
          emulator->continueWithKey(key);
      }
    }

    window->setDebugInfo(emulator->getReg(), emulator->getKeypad(),
                         emulator->getState(), emulator->getDelayTimer(),
                         emulator->getSoundTimer());
    window->draw();
  }
  delete window;
}
