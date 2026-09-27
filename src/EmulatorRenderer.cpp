
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

  painter.setPen(QColor(fg.red, fg.green, fg.blue));
  painter.setBrush(QColor(fg.red, fg.green, fg.blue));

  for(size_t i = 0; i < m_framebuffer.size(); i++) {
    if(m_framebuffer[i]) painter.drawRect(_x(i) * dpi, _y(i) * dpi, dpi, dpi);
  }
  
}
