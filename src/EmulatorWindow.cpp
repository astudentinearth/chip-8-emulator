
#include "EmulatorWindow.hpp"
#include <QPalette>

QtEmulatorWindow::QtEmulatorWindow(QWidget *parent, AppContext &context)
    : QWidget(parent), m_context{context} {
  setWindowTitle(QString::fromStdString(context.getWindowTitle()));
  resize(DefaultWidth, DefaultHeight);
  setMinimumSize(DefaultWidth / 2, DefaultHeight / 2);

  m_emulator = new EmulatorSource(context, this);
  m_toolbar = new EmulatorToolbar(context, this);
  m_debugger = new EmulatorDebugger(this);

  m_renderer = new EmulatorRenderer(this, chip8::Framebuffer{});
  m_renderer->setColorScheme(context.colorscheme);
  
  m_layout = new QVBoxLayout(this);
  m_layout->setContentsMargins(0, 0, 0, 0);
  m_layout->setMenuBar(m_toolbar);
  m_layout->addWidget(m_renderer, 1);
  m_layout->addWidget(m_debugger);
  m_renderer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

  connect(m_emulator, &EmulatorSource::emulatorRedraw, m_renderer,
          &EmulatorRenderer::setFrame, Qt::QueuedConnection);

  connect(m_emulator, &EmulatorSource::emulatorStateChanged, m_toolbar,
          &EmulatorToolbar::setEmulatorState, Qt::QueuedConnection);

  connect(m_emulator, &EmulatorSource::registersChanged, m_debugger,
          &EmulatorDebugger::setRegisters, Qt::QueuedConnection);

  connect(m_emulator, &EmulatorSource::ispChanged, m_debugger,
          &EmulatorDebugger::setIsp, Qt::QueuedConnection);

  connect(m_renderer, &EmulatorRenderer::keyPress, m_emulator,
          &EmulatorSource::setKeyPressed );

  connect(m_renderer, &EmulatorRenderer::keyRelease, m_emulator,
          &EmulatorSource::setKeyReleased );
}

QtEmulatorWindow::~QtEmulatorWindow() {
  m_context.emulator->hlt();
  m_context.emulator->display()->onDraw([](chip8::Framebuffer _) {});
}
