
#pragma once
#include <array>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stack>

using namespace std;

namespace chip8 {
void hello();

constexpr int CHIP8_DISPLAY_WIDTH = 64;  
constexpr int CHIP8_DISPLAY_HEIGHT = 32;
constexpr int CHIP8_FONT_SET_SIZE = 80;  // bytes

const uint8_t fontset[CHIP8_FONT_SET_SIZE]{
    0xf0, 0x90, 0x90, 0x90, 0xf0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xf0, 0x10, 0xf0, 0x80, 0xf0,  // 2
    0xf0, 0x10, 0xf0, 0x10, 0xf0,  // 3
    0x90, 0x90, 0xf0, 0x10, 0x10,  // 4
    0xf0, 0x80, 0xf0, 0x10, 0xf0,  // 5
    0xf0, 0x80, 0xf0, 0x90, 0xf0,  // 6
    0xf0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xf0, 0x90, 0xf0, 0x90, 0xf0,  // 8
    0xf0, 0x90, 0xf0, 0x10, 0xf0,  // 9
    0xf0, 0x90, 0xf0, 0x90, 0x90,  // A
    0xe0, 0x90, 0xe0, 0x90, 0xe0,  // B
    0xf0, 0x80, 0x80, 0x80, 0xf0,  // C
    0xe0, 0x90, 0x90, 0x90, 0xe0,  // D
    0xf0, 0x80, 0xf0, 0x80, 0xf0,  // E
    0xf0, 0x80, 0xf0, 0x80, 0x80   // F
};

constexpr uint8_t CharGlyphOffset(uint8_t ch) { return ch * 5; }

using Framebuffer = array<uint8_t, CHIP8_DISPLAY_WIDTH * CHIP8_DISPLAY_HEIGHT>;

class Chip8Display {
 public:
  typedef void (*onDrawFn)(const Framebuffer& buf);
  explicit Chip8Display() = default;
  explicit Chip8Display(onDrawFn onDraw);
  static unique_ptr<Chip8Display> create() {
    return make_unique<Chip8Display>();
  }
  static unique_ptr<Chip8Display> create(onDrawFn onDraw) {
    return make_unique<Chip8Display>(onDraw);
  }
  void redraw() const;
  void clear();
  bool drawByte(int x, int y, uint8_t byte);
  Framebuffer& getFramebuffer() { return m_framebuffer; };

 private:
  Framebuffer m_framebuffer;
  onDrawFn m_onDraw = [](const Framebuffer& _) {};
};

struct Registers {
  uint8_t v0{0};
  uint8_t v1{0};
  uint8_t v2{0};
  uint8_t v3{0};
  uint8_t v4{0};
  uint8_t v5{0};
  uint8_t v6{0};
  uint8_t v7{0};
  uint8_t v8{0};
  uint8_t v9{0};
  uint8_t va{0};
  uint8_t vb{0};
  uint8_t vc{0};
  uint8_t vd{0};
  uint8_t ve{0};

  /** delay timer */
  uint8_t dt{0};

  /** sound timer */
  uint8_t st{0};

  /** carry flag */
  uint8_t vf{0};

  /** the register getkey instruction should store the key code in */
  uint16_t k{0};

  /** address register, last 4 bits unused */
  uint16_t i;
  uint8_t& operator[](uint16_t idx);
};

using Keypad = std::array<bool, 16>;

namespace op {

constexpr uint16_t Address(uint16_t code) { return (code & 0x0FFF); }
constexpr uint16_t CondReg(uint16_t code) { return (code & 0x0F00) >> 8; }
constexpr uint16_t CondVal(uint16_t code) { return (code & 0x00FF); }
constexpr uint16_t ConstSetReg(uint16_t code) { return (code & 0x0F00) >> 8; }
constexpr uint16_t ConstSetVal(uint16_t code) { return (code & 0x00FF); }
constexpr uint16_t ConstAddReg(uint16_t code) { return ConstSetReg(code); }
constexpr uint16_t ConstAddVal(uint16_t code) { return ConstSetVal(code); }
constexpr uint16_t LeftReg(uint16_t code) { return (code & 0x0F00) >> 8; }
constexpr uint16_t RightReg(uint16_t code) { return (code & 0x00F0) >> 4; }
constexpr uint16_t BinOpType(uint16_t code) { return (code & 0x000F); }
constexpr uint16_t DrawHeight(uint16_t code) { return (code & 0x000F); }

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

enum MiscOpType : uint16_t {
  GetDelayTimer = 0x07,
  GetKey = 0x0A,
  SetDelayTimer = 0x15,
  SetSoundTimer = 0x18,
  AddVxToI = 0x1E,
  SetCharSprite = 0x29,
  BCD = 0x33,
  RegDump = 0x55,
  RegLoad = 0x65
};

constexpr uint16_t OpClassMask = 0xF000;
constexpr uint16_t MiscOpTypeMask = 0x00FF;
constexpr uint16_t KeyCondEqMask = 0x009E;
constexpr uint16_t KeyCondNotEqMask = 0x00A1;
constexpr bool IsHcf(uint16_t opcode, uint16_t isp) {
    if((opcode & Jump) && Address(opcode) == isp) return true;
    return false;
}

}  // namespace op

enum class EmulatorState {
    Running,
    WaitingInput,
    Halted
};

std::ostream& operator <<(std::ostream& os, EmulatorState state);

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
  void setKey(uint8_t key, bool state);

  /** Executes passed instruction immediately.
   * @return true if execution altered the program counter */
  bool eval(uint16_t opcode);
  /** Executes the instruction at program counter.
   * @return true if execution altered the program counter */
  bool exec();

  EmulatorState run();
  void continueWithKey(uint8_t key);
  EmulatorState getState() const { return m_state; }
  void hlt() { m_state = EmulatorState::Halted; }
  Registers getReg() const { return m_reg; }
  Keypad getKeypad() const { return m_keypad; }

  void dumpState() const;

 private:
  bool evalMathOp(uint16_t opcode);
  bool evalMiscOp(uint16_t opcode);
  Registers m_reg{};

  /** call stack */
  stack<uint16_t> m_stack{};

  /** address space */
  uint8_t memory[MemorySize]{};

  size_t m_size;

  /** program counter */
  uint16_t isp{ProgramStart};
  Chip8Display* m_display;
  Keypad m_keypad{};
  EmulatorState m_state{EmulatorState::Halted};
};
}  // namespace chip8
