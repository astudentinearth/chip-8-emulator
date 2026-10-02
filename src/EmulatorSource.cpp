#include "EmulatorWindow.hpp"
#include <iostream>

EmulatorSource::EmulatorSource(AppContext &context, QObject *parent)
    : QObject(parent), m_context{context} {
  m_buzzer = new Buzzer(this);
  auto em = context.emulator.get();
  em->onStateChanged([this](auto st) { emit emulatorStateChanged(st); });
  em->display()->onDraw([this](auto fb) { emit emulatorRedraw(fb); });
  em->onInstructionExecuted([this](auto reg, auto isp) {
    emit(registersChanged(reg));
    emit(ispChanged(isp));
  });

  em->onBeep([this]() { emit beep(); });
  connect(
      this, &EmulatorSource::beep, this,
      [this]() {
        m_buzzer->play();
      },
      Qt::QueuedConnection);
};

EmulatorSource::~EmulatorSource() {
  auto emulator = m_context.emulator.get();

  // cleanup listeners
  emulator->onStateChanged([](chip8::EmulatorState st) {});
  emulator->onInstructionExecuted([](auto reg, auto isp) {});
  emulator->display()->onDraw([](chip8::Framebuffer fb) {});
  emulator->onBeep([]() {});
}
