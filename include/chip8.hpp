
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
  void clear();

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
  uint8_t& operator[](uint16_t idx);
};

namespace op {

enum OpMask : uint16_t {
  Address = 0x0FFF,
  CondReg = 0x0F00,
  CondVal = 0x00FF,
  ConstSetReg = 0x0F00,
  ConstSetVal = 0x00FF,
  ConstAddReg = ConstSetReg,
  ConstAddVal = ConstSetVal,
  LeftReg = 0x0F00,
  RightReg = 0x00F0,
  BinOpType = 0x000F,
  DrawHeight = 0x000F
};

enum FixedCode : uint16_t { ClearDisplay = 0x00E0, Return = 0x00EE };

enum OpClass : uint16_t {
  Jump = 0x1000,
  Call = 0x2000,
  SkipIfEqual = 0x3000,
  SkipIfNotEqual = 0x4000,
  SkipIfRegEqual = 0x5000,
  ConstSet = 0x6000,
  ConstAdd = 0x7000,
  Math = 0x8000,
  SkipIfRegNotEqual = 0x9000,
  SetAddressReg = 0xA000,
  OffsetJump = 0xB000,
  Random = 0xC000,
  Draw = 0xD000,
  KeyCond = 0xE000,
  Misc = 0xF000
};

enum MathOpType : uint16_t {
  Assignment = 0x0,
  Or = 0x1,
  And = 0x2,
  Xor = 0x3,
  Add = 0x4,
  Sub = 0x5,
  RightShift = 0x6,
  Difference = 0x7,
  LeftShift = 0xE
};

constexpr uint16_t OpClassMask = 0xF000;

}  // namespace op

class Chip8Emulator {
 public:
  explicit Chip8Emulator() = default;
  enum : int {
    MemorySize = 4096,
    ProgramStart = 0x200,
    MaxProgramSize = ProgramStart - MemorySize
  };

  static unique_ptr<Chip8Emulator> create(Chip8Display* display);

  bool loadProgram(const char* program, size_t size);
  uint16_t fetch(const uint16_t addr) const;
  uint16_t fetchNext();
  uint16_t getIsp() const;
  uint16_t ret();
  void call(uint16_t addr);
  void jmp(uint16_t addr);
  uint8_t& getReg(uint8_t idx);

  /** Executes passed instruction immediately.
   * @return true if execution altered the program counter */
  bool eval(uint16_t opcode);
  /** Executes the instruction at program counter.
   * @return true if execution altered the program counter */
  bool exec();
  void dumpState() const;

 private:
  Registers m_reg{};

  /** call stack */
  stack<uint16_t> m_stack{};

  /** address space */
  uint8_t memory[MemorySize / 2]{};

  size_t m_size;

  /** program counter */
  uint16_t isp{ProgramStart};
  Chip8Display* m_display;
};
}  // namespace chip8
