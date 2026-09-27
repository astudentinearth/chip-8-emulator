
#include "EmulatorWindow.hpp"

EmulatorRenderer::EmulatorRenderer(QWidget *parent,
                                   chip8::Framebuffer framebuffer)
    : QWidget(parent), m_framebuffer{framebuffer} {
  m_label = new QLabel(this);
  m_label->setText(QString("Counter: %1").arg(counter));
  m_label->setStyleSheet("font-size: 24px; ");

  auto layout = new QVBoxLayout(this);
  layout->addWidget(m_label);
};
