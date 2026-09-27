
#include "App.hpp"
#include "Color.hpp"
#include "Window.hpp"
#include "chip8.hpp"
#include <QtWidgets/qwidget.h>
#include <SDL3/SDL.h>
#include <string>
#include <QWidget>
#include <QApplication>
#include "EmulatorWindow.hpp"

using namespace std;
using namespace chip8;

void runSDLApp(AppContext &context) {
  SDL_Init(SDL_INIT_VIDEO);
  EmulatorWindow window(&context.framebuffer);
  window.setTitle("CHIP-8 Emulator ("s + context.filename+ ")");
  window.setColorScheme(context.colorscheme);
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

    window.setDebugInfo(
        context.emulator->getReg(), context.emulator->getKeypad(),
        context.emulator->getState(), context.emulator->getDelayTimer(),
        context.emulator->getSoundTimer());
    window.draw();
  }
}


int runQt6App(AppContext &context) {
  QApplication app(context.argc, const_cast<char**>(context.argv));
  QtEmulatorWindow window(nullptr, context);
  window.show();
  context.window_ready.set_value();
  return app.exec();
}

