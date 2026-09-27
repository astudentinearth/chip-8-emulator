#include "EmulatorWindow.hpp"

EmulatorSource::EmulatorSource(AppContext &context, QObject *parent)
    : QObject(parent), m_context{context} {
  auto em = context.emulator;
  em->onStateChanged([this](auto st) { emit emulatorStateChanged(st); });
  em->display()->onDraw([this](auto fb) { emit emulatorRedraw(fb); });
  em->onInstructionExecuted([this](auto reg, auto isp) {
    emit(registersChanged(reg));
    emit(ispChanged(isp));
  });
};

EmulatorSource::~EmulatorSource() {
  auto emulator = m_context.emulator;

  // cleanup listeners
  emulator->onStateChanged([](chip8::EmulatorState st) {});
  emulator->onInstructionExecuted([](auto reg, auto isp) {});
  emulator->display()->onDraw([](chip8::Framebuffer fb) {});
}
