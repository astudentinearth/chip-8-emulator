#include "Window.hpp"

#include <cstdio>
#include <stdexcept>

#include "SDL3/SDL_rect.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "chip8.hpp"

EmulatorWindow::EmulatorWindow(chip8::Framebuffer* fb) {
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
  float wpp =
      static_cast<float>(width - DebugPaneWidth) / (chip8::CHIP8_DISPLAY_WIDTH);
  float hpp = static_cast<float>(height) / chip8::CHIP8_DISPLAY_HEIGHT;
  float dpi = wpp > hpp ? hpp : wpp;

  SDL_FRect rect{.x = 0.0, .y = 0.0, .w = dpi, .h = dpi};

  for (int y = 0; y < chip8::CHIP8_DISPLAY_HEIGHT; y++) {
    for (int x = 0; x < chip8::CHIP8_DISPLAY_WIDTH; x++) {
      rect.x = x * dpi + DebugPaneWidth;
      rect.y = y * dpi;
      bool px = (*m_fb)[(y * chip8::CHIP8_DISPLAY_WIDTH) + x];
      if (!px) continue;
      ;
      SDL_RenderFillRect(m_renderer, &rect);
    }
  }

  uint32_t debugY = 4;

  for (uint16_t i = 0; i < 16; i++) {
    char buf[32]{};
    snprintf(buf, 32, "v%x: 0x%x | %d", i, m_reg[i], m_reg[i]);
    SDL_RenderDebugText(m_renderer, 4, debugY, buf);
    debugY += 12;
  }

  debugY += 12;
  SDL_RenderDebugText(m_renderer, 4, debugY, "== Keypad ==");
  debugY += 12;

  for (uint16_t i = 0; i < 16; i++) {
    char buf[32]{};
    snprintf(buf, 32, "%x: %s", i, m_keypad[i] ? "on" : "off");
    SDL_RenderDebugText(m_renderer, 4, debugY, buf);
    debugY += 12;
  }

  debugY += 12;
  SDL_RenderDebugText(m_renderer, 4, debugY, "== Timers ==");
  debugY += 12;
  char delay[32]{};
  char sound[32]{};
  snprintf(delay, 32, "Delay: %d", m_dt);
  snprintf(sound, 32, "Sound: %d", m_st);
  SDL_RenderDebugText(m_renderer, 4, debugY, delay);
  debugY += 12;
  SDL_RenderDebugText(m_renderer, 4, debugY, sound);
  debugY += 12;
  SDL_FRect debugPane{
      .x = 0, .y = 0, .w = DebugPaneWidth, .h = static_cast<float>(height)};
  SDL_RenderRect(m_renderer, &debugPane);

  SDL_FRect displayPane{.x = DebugPaneWidth - 1,
                        .y = 0,
                        .w = dpi * chip8::CHIP8_DISPLAY_WIDTH,
                        .h = dpi * chip8::CHIP8_DISPLAY_HEIGHT};
  SDL_RenderRect(m_renderer, &displayPane);

  SDL_RenderPresent(m_renderer);
}

void EmulatorWindow::setTitle(std::string title) {
  SDL_SetWindowTitle(m_window, title.c_str());
}
