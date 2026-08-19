#include <SDL3/SDL.h>

#include "chip8.hpp"


class EmulatorWindow {
 public:
  enum { StartingWidth = 800, StartingHeight = 400 };
  explicit EmulatorWindow(chip8::Framebuffer& fb);
  void draw();
  ~EmulatorWindow();

 private:
  SDL_Window* m_window = nullptr;
  SDL_Renderer* m_renderer = nullptr;
  chip8::Framebuffer m_fb{};
};
