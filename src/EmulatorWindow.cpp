
#include "EmulatorWindow.hpp"

QtEmulatorWindow::QtEmulatorWindow(QWidget *parent, AppContext &context)
    : QWidget(parent), m_context{context} {
  setWindowTitle(QString::fromStdString(context.getWindowTitle()));
  resize(DefaultWidth, DefaultHeight);
  setMinimumSize(DefaultWidth / 2, DefaultHeight / 2);

  m_renderer = new EmulatorRenderer(this, chip8::Framebuffer{});

  m_layout = new QVBoxLayout(this);
  m_layout->setContentsMargins(0, 0, 0, 0);
  m_layout->addWidget(m_renderer, 1, Qt::AlignTop);

  context.emulator->display()->onDraw(
      [this](chip8::Framebuffer fb) { emit redraw(fb); });
  connect(this, &QtEmulatorWindow::redraw, m_renderer,
          &EmulatorRenderer::setFrame, Qt::QueuedConnection);
}

QtEmulatorWindow::~QtEmulatorWindow() {
    m_context.emulator->hlt();
    m_context.emulator->display()->onDraw([](chip8::Framebuffer _){});
}
