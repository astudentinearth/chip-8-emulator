
#include <cstring>

#include "chip8.hpp"

namespace chip8 {
Chip8Display::Chip8Display(onDrawFn onDraw) { m_onDraw = onDraw; }

bool Chip8Emulator::loadProgram(const char *program, size_t size) {
  if (size > MaxProgramSize) return false;
  memcpy(&memory[ProgramStart], program, size);
  return true;
}
}  // namespace chip8
