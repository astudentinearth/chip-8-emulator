
#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stack>
#define CHIP8_DISPLAY_WIDTH 8
#define CHIP8_DISPLAY_HEIGHT 4

using namespace std;

namespace chip8 {
void hello();

class Chip8Display {
 public:
  typedef void (*onDrawFn)();
  explicit Chip8Display() = default;
  explicit Chip8Display(onDrawFn onDraw);
  static unique_ptr<Chip8Display> create() {
    return make_unique<Chip8Display>();
  }

 private:
  uint8_t m_framebuffer[CHIP8_DISPLAY_HEIGHT][CHIP8_DISPLAY_WIDTH];
  onDrawFn m_onDraw = []() {};
};

struct Registers {
  uint8_t v0;
  uint8_t v1;
  uint8_t v2;
  uint8_t v3;
  uint8_t v4;
  uint8_t v5;
  uint8_t v6;
  uint8_t v7;
  uint8_t v8;
  uint8_t v9;
  uint8_t va;
  uint8_t vb;
  uint8_t vc;
  uint8_t vd;
  uint8_t ve;

  /** carry flag */
  uint8_t vf;

  /** address register, last 4 bits unused */
  uint16_t i;
};

class Chip8Emulator {
 public:
  explicit Chip8Emulator() = default;
  enum : int { MemorySize = 4096, ProgramStart = 0x200, MaxProgramSize = ProgramStart - MemorySize };

  static unique_ptr<Chip8Emulator> create(Chip8Display* display) {
    auto emulator = make_unique<Chip8Emulator>();
    emulator->m_display = display;
    return emulator;
  }

  bool loadProgram(const char* program, size_t size);

 private:
  Registers m_reg{};

  /** call stack */
  stack<uint16_t> m_stack{};

  /** address space */
  uint8_t memory[MemorySize]{};

  /** program counter */
  uint16_t isp{ProgramStart};
  Chip8Display* m_display;
};
}  // namespace chip8
