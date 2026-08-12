
#include <cstdio>
#include <cstring>

#include "chip8.hpp"

namespace chip8 {
Chip8Display::Chip8Display(onDrawFn onDraw) { m_onDraw = onDraw; }

unique_ptr<Chip8Emulator> Chip8Emulator::create(Chip8Display* display) {
  auto emulator = make_unique<Chip8Emulator>();
  emulator->m_display = display;
  return emulator;
}

bool Chip8Emulator::loadProgram(const char* program, size_t size) {
  if (size > MaxProgramSize) return false;
  memcpy(&memory[ProgramStart], program, size);
  m_size = size;
  return true;
}

uint16_t Chip8Emulator::fetch(const uint16_t addr) const {
  const uint8_t high = memory[addr];
  const uint8_t low = memory[addr + 1];
  return high << 8 | low;
}

uint16_t Chip8Emulator::fetchNext() {
  uint16_t val = fetch(isp);
  isp += 2;
  return val;
}

uint16_t Chip8Emulator::getIsp() const { return isp; }

template<typename T>
static void dumpreg_(const char* reg, T value) {
    printf("%s: 0x%X\n", reg, value);
}

void Chip8Emulator::dumpState() const {
    const Registers reg = m_reg;
    dumpreg_("v0", reg.v0);
    dumpreg_("v1", reg.v1);
    dumpreg_("v2", reg.v2);
    dumpreg_("v3", reg.v3);
    dumpreg_("v4", reg.v4);
    dumpreg_("v5", reg.v5);
    dumpreg_("v6", reg.v6);
    dumpreg_("v7", reg.v7);
    dumpreg_("v8", reg.v8);
    dumpreg_("v9", reg.v9);
    dumpreg_("va", reg.va);
    dumpreg_("vb", reg.vb);
    dumpreg_("vc", reg.vc);
    dumpreg_("vd", reg.vd);
    dumpreg_("ve", reg.ve);
    dumpreg_("vf", reg.vf);
    dumpreg_("i", reg.i);
    dumpreg_("isp", isp);
    dumpreg_("opcode", fetch(isp));
}

}  // namespace chip8
