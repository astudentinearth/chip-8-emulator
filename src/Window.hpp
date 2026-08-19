#include <SDL3/SDL.h>

#include "chip8.hpp"


class EmulatorWindow {
 public:
  enum { StartingWidth = 800, StartingHeight = 400, DebugPaneWidth = 150 };
  explicit EmulatorWindow(chip8::Framebuffer* fb);
  void draw();
  void setDebugInfo(chip8::Registers reg, chip8::Keypad keypad, chip8::EmulatorState state) {
    m_reg = reg;
    m_keypad = keypad;
    m_emstate = state;
  }
  ~EmulatorWindow();

 private:
  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;
  chip8::Framebuffer *m_fb;
  chip8::Registers m_reg{};
  chip8::Keypad m_keypad{};
  chip8::EmulatorState m_emstate;
};
