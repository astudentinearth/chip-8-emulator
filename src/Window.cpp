#include "Window.hpp"

#include <cstdio>
#include <stdexcept>

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "chip8.hpp"

EmulatorWindow::EmulatorWindow(chip8::Framebuffer& fb) {
  m_fb = fb;
  if (!SDL_CreateWindowAndRenderer("main", StartingWidth, StartingHeight,
                                   SDL_WINDOW_RESIZABLE, &m_window,
                                   &m_renderer)) {
    throw runtime_error("Failed to initialize SDL window.");
  }
}

EmulatorWindow::~EmulatorWindow() {
  SDL_DestroyRenderer(m_renderer);
  SDL_DestroyWindow(m_window);
}

void EmulatorWindow::draw() {
  SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
  SDL_RenderClear(m_renderer);
  SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
  int width{0}, height{0};
  SDL_GetWindowSize(m_window, &width, &height);
  float wpp = static_cast<float>(width - DebugPaneWidth) / (chip8::CHIP8_DISPLAY_WIDTH);
  float hpp = static_cast<float>(height) / chip8::CHIP8_DISPLAY_HEIGHT;

  SDL_FRect rect{.x = 0.0,
                 .y = 0.0,
                 .w = static_cast<float>(wpp),
                 .h = static_cast<float>(hpp)};

  for (int y = 0; y < chip8::CHIP8_DISPLAY_HEIGHT; y++) {
    for (int x = 0; x < chip8::CHIP8_DISPLAY_WIDTH; x++) {
      rect.x = x * wpp + DebugPaneWidth;
      rect.y = y * hpp;
      bool px = m_fb[(y * chip8::CHIP8_DISPLAY_WIDTH) + x];
      if (!px) continue;
      ;
      SDL_RenderFillRect(m_renderer, &rect);
    }
  }

  uint32_t debugY = 4;

  for(uint16_t i = 0; i < 16; i++) {
      char buf[32]{};
      snprintf(buf, 32, "v%x: 0x%x | %d", i, m_reg[i], m_reg[i]);
      SDL_RenderDebugText(m_renderer, 4, debugY, buf);
      debugY += 12;
  }

      debugY += 12;
  SDL_RenderDebugText(m_renderer, 4, debugY, "== Keypad == ");
  debugY += 12;

  for(uint16_t i = 0; i < 16; i++) {
      char buf[32]{};
      snprintf(buf, 32, "%x: %s", i, m_keypad[i] ? "on" : "off");
      SDL_RenderDebugText(m_renderer, 4, debugY, buf);
      debugY += 12;
  }

  SDL_FRect debugPane{.x=2, .y=2, .w = DebugPaneWidth - 4, .h = static_cast<float>(height - 4)};
  SDL_RenderRect(m_renderer, &debugPane);

  SDL_RenderPresent(m_renderer);
}
