
#include "EmulatorWindow.hpp"
#include <QKeyEvent>
#include <QPainter>
#include <unordered_map>

static const auto keymap = std::unordered_map<int, uint8_t>{
  {Qt::Key_1, 1},
  {Qt::Key_2, 2},
  {Qt::Key_3, 3},
  {Qt::Key_4, 0xC},
  {Qt::Key_Q, 4},
  {Qt::Key_W, 5},
  {Qt::Key_E, 6},
  {Qt::Key_R, 0xD},
  {Qt::Key_A, 7},
  {Qt::Key_S, 8},
  {Qt::Key_D, 9},
  {Qt::Key_F, 0xE},
  {Qt::Key_Z, 0xA},
  {Qt::Key_X, 0},
  {Qt::Key_C, 0xB},
  {Qt::Key_V, 0xF},
};

static constexpr size_t _x(size_t idx) {
  return idx % chip8::CHIP8_DISPLAY_WIDTH;
}
static constexpr size_t _y(size_t idx) {
  return idx / chip8::CHIP8_DISPLAY_WIDTH;
}

EmulatorRenderer::EmulatorRenderer(QWidget *parent,
                                   chip8::Framebuffer framebuffer)
    : QWidget(parent), m_framebuffer{framebuffer} {
  this->setFocusPolicy(Qt::FocusPolicy::StrongFocus);
    };

void EmulatorRenderer::paintEvent(QPaintEvent *event) {
  QPainter painter(this);
  auto bg = m_colorscheme.background;
  auto fg = m_colorscheme.foreground;
  painter.fillRect(rect(), QColor(bg.red, bg.green, bg.blue));

  float wpp = static_cast<float>(width()) / chip8::CHIP8_DISPLAY_WIDTH;
  float hpp = static_cast<float>(height()) / chip8::CHIP8_DISPLAY_HEIGHT;
  float dpi = min(wpp, hpp);

  float aspect_ratio =
      static_cast<float>(width()) / static_cast<float>(height());
  float x_offset =
      (static_cast<float>(width()) - (dpi * chip8::CHIP8_DISPLAY_WIDTH)) / 2;
  float y_offset =
      (static_cast<float>(height()) - (dpi * chip8::CHIP8_DISPLAY_HEIGHT)) / 2;

  painter.setPen(QColor(fg.red, fg.green, fg.blue));
  painter.setBrush(QColor(fg.red, fg.green, fg.blue));

  for (size_t i = 0; i < m_framebuffer.size(); i++) {
    if (m_framebuffer[i])
      painter.drawRect(x_offset + _x(i) * dpi, y_offset + _y(i) * dpi, dpi,
                       dpi);
  }
}

static constexpr uint8_t KEY_IGNORE = 99;
static uint8_t infer_key(int keycode) {
  uint8_t key = KEY_IGNORE;
  if(keymap.contains(keycode)) {
    return keymap.at(keycode);
  }
  if (keycode >= Qt::Key_0 && keycode <= Qt::Key_9) {
    key = (keycode - Qt::Key_0) & 0xFF;
  }
  if (keycode >= Qt::Key_A && keycode <= Qt::Key_F) {
    key = (keycode - Qt::Key_A + 0xA) & 0xFF;
  }
  return key;
}

void EmulatorRenderer::keyPressEvent(QKeyEvent *event) {
  uint8_t key = infer_key(event->key());
  if (key == KEY_IGNORE)
    return;
  emit keyPress(key);
}

void EmulatorRenderer::keyReleaseEvent(QKeyEvent *event) {
  uint8_t key = infer_key(event->key());
  if (key == KEY_IGNORE)
    return;
  emit keyRelease(key);
}
