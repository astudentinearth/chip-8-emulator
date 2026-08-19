#include <SDL3/SDL.h>

#include "chip8.hpp"


class EmulatorWindow {
 public:
  enum { StartingHeight = 450, DebugPaneWidth = 150, StartingWidth = DebugPaneWidth + 900 };
  explicit EmulatorWindow(chip8::Framebuffer* fb);
  void draw();
  void setDebugInfo(chip8::Registers reg, chip8::Keypad keypad, chip8::EmulatorState state, uint8_t dt, uint8_t st) {
    m_reg = reg;
    m_keypad = keypad;
    m_emstate = state;
    m_dt = dt;
    m_st = st;
  }
  void setTitle(std::string title);
  ~EmulatorWindow();

 private:
  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;
  chip8::Framebuffer *m_fb;
  chip8::Registers m_reg{};
  chip8::Keypad m_keypad{};
  chip8::EmulatorState m_emstate;
  uint8_t m_dt{};
  uint8_t m_st{};
};
