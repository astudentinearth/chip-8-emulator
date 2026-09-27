
#include "EmulatorWindow.hpp"
#include <QPainter>
#include <QtGui/qicon.h>

static constexpr size_t _x(size_t idx) { return idx % chip8::CHIP8_DISPLAY_WIDTH; }
static constexpr size_t _y(size_t idx) { return idx / chip8::CHIP8_DISPLAY_WIDTH; }

EmulatorRenderer::EmulatorRenderer(QWidget *parent,
                                   chip8::Framebuffer framebuffer)
    : QWidget(parent), m_framebuffer{framebuffer} {};

void EmulatorRenderer::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  auto bg = m_colorscheme.background;
  auto fg = m_colorscheme.foreground;
  painter.fillRect(rect(), QColor(bg.red, bg.green, bg.blue));

  float wpp = static_cast<float>(width()) / chip8::CHIP8_DISPLAY_WIDTH;
  float hpp = static_cast<float>(height()) / chip8::CHIP8_DISPLAY_HEIGHT;
  float dpi = min(wpp, hpp);

  float aspect_ratio = static_cast<float>(width()) / static_cast<float>(height());
  float x_offset = (static_cast<float>(width()) - (dpi * chip8::CHIP8_DISPLAY_WIDTH)) / 2;
  float y_offset = (static_cast<float>(height()) - (dpi * chip8::CHIP8_DISPLAY_HEIGHT)) / 2;

  painter.setPen(QColor(fg.red, fg.green, fg.blue));
  painter.setBrush(QColor(fg.red, fg.green, fg.blue));

  for(size_t i = 0; i < m_framebuffer.size(); i++) {
    if(m_framebuffer[i]) painter.drawRect(x_offset + _x(i) * dpi, y_offset + _y(i) * dpi, dpi, dpi);
  }
  
}
