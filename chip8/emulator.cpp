
#include <cstdio>
#include <cstring>

#include "chip8.hpp"
#include "util.hpp"

namespace chip8 {
Chip8Display::Chip8Display(onDrawFn onDraw) { m_onDraw = onDraw; }

constexpr int framebufferIdx(int x, int y) {
  return (y * CHIP8_DISPLAY_HEIGHT) + x;
}

unique_ptr<Chip8Emulator> Chip8Emulator::create(Chip8Display* display) {
  auto emulator = make_unique<Chip8Emulator>();
  emulator->m_display = display;
  return emulator;
}

void Chip8Display::clear() {
  Framebuffer buf{};
  m_framebuffer = buf;
}

void Chip8Display::redraw() const { m_onDraw(m_framebuffer); }
bool Chip8Display::drawByte(int x, int y, uint8_t byte) {
  auto current = m_framebuffer[framebufferIdx(x, y)];
  m_framebuffer[framebufferIdx(x, y)] ^= byte;
  return byte & current;  // whether we unset any bits
}

bool Chip8Emulator::loadProgram(const char* program, size_t size) {
  if (size > MaxProgramSize) return false;
  memcpy(&memory[ProgramStart], program, size);
  memcpy(&memory[0], fontset, CHIP8_FONT_SET_SIZE);
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

template <typename T>
static void dumpreg_(const char* reg, T value) {
  printf("%s: 0x%X\n", reg, value);
}

uint8_t& Registers::operator[](uint16_t idx) {
  switch (idx) {
    case 0:
      return v0;
    case 1:
      return v1;
    case 2:
      return v2;
    case 3:
      return v3;
    case 4:
      return v4;
    case 5:
      return v5;
    case 6:
      return v6;
    case 7:
      return v7;
    case 8:
      return v8;
    case 9:
      return v9;
    case 10:
      return va;
    case 11:
      return vb;
    case 12:
      return vc;
    case 13:
      return vd;
    case 14:
      return ve;
    case 15:
      return vf;
  }
  return v0;
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

uint16_t Chip8Emulator::ret() {
  uint16_t addr = m_stack.top();
  m_stack.pop();
  isp = addr;
  return isp;
}

void Chip8Emulator::jmp(uint16_t addr) { isp = addr; }

void Chip8Emulator::call(uint16_t addr) {
  m_stack.push(isp + 1);
  jmp(addr);
}

void Chip8Emulator::setKey(uint8_t key, bool state) {
  if (key > 15) return;
  m_keypad[key] = state;
}

bool Chip8Emulator::evalMathOp(uint16_t opcode) {
  switch (op::BinOpType(opcode)) {
    case op::Assignment: {
      m_reg[op::LeftReg(opcode)] = m_reg[op::RightReg(opcode)];
      break;
    }

    case op::Or: {
      m_reg[op::LeftReg(opcode)] |= m_reg[op::RightReg(opcode)];
      break;
    }

    case op::And: {
      m_reg[op::LeftReg(opcode)] &= m_reg[op::RightReg(opcode)];
      break;
    }

    case op::Xor: {
      m_reg[op::LeftReg(opcode)] ^= m_reg[op::RightReg(opcode)];
      break;
    }

    case op::Add: {
      auto& left = m_reg[op::LeftReg(opcode)];
      auto original = left;
      left += m_reg[op::RightReg(opcode)];
      if (left < original)
        m_reg.vf = 1;
      else
        m_reg.vf = 0;
      break;
    }

    case op::Sub: {
      auto& left = m_reg[op::LeftReg(opcode)];
      auto original = left;
      left -= m_reg[op::RightReg(opcode)];
      if (left > original)
        m_reg.vf = 0;
      else
        m_reg.vf = 1;
      break;
    }

    case op::LeftShift: {
      auto& reg = m_reg[op::LeftReg(opcode)];
      m_reg.vf = 0x8000 & reg;  // extract least significant bit
      reg <<= 1;
      break;
    }

    case op::RightShift: {
      auto& reg = m_reg[op::LeftReg(opcode)];
      m_reg.vf = 0x1 & reg;
      reg >>= 1;
      break;
    }

    case op::Difference: {
      auto& vy = m_reg[op::RightReg(opcode)];
      auto& vx = m_reg[op::LeftReg(opcode)];
      auto result = vy - vx;
      if (vy >= vx)
        m_reg.vf = 1;
      else
        m_reg.vf = 0;
      vx = vy - vx;
      break;
    }
  }
  return false;
}

bool Chip8Emulator::evalMiscOp(uint16_t opcode) {
  switch (opcode & op::MiscOpTypeMask) {
    case op::GetKey: {
      m_reg.k = op::LeftReg(opcode);
      return false;
    }

    case op::GetDelayTimer:
      m_reg[op::LeftReg(opcode)] = m_reg.dt;
      return false;

    case op::SetDelayTimer:
      m_reg.dt = m_reg[op::LeftReg(opcode)];
      return false;

    case op::SetSoundTimer:
      m_reg.st = m_reg[op::LeftReg(opcode)];
      return false;

    case op::AddVxToI:
      m_reg.i += m_reg[op::LeftReg(opcode)];
      return false;

    case op::SetCharSprite:
      m_reg.i = CharGlyphOffset(m_reg[op::LeftReg(opcode)]);
      return false;

    case op::BCD: {
      auto num = m_reg[op::LeftReg(opcode)];
      memory[m_reg.i] = num / 100;
      memory[m_reg.i + 1] = (num / 10) % 10;
      memory[m_reg.i + 2] = num % 10;
      return false;
    }

    case op::RegDump: {
      for (uint8_t vx = 0; vx <= op::LeftReg(opcode); vx++) {
        memory[m_reg.i++] = m_reg[vx];
      }
      return false;
    }

    case op::RegLoad: {
      for (uint8_t vx = 0; vx <= op::LeftReg(opcode); vx++) {
        m_reg[vx] = memory[m_reg.i++];
      }
      return false;
    }
  }
  return false;
}

bool Chip8Emulator::eval(uint16_t opcode) {
  printf("%X: ", opcode);
  switch (opcode & op::OpClassMask) {
    case 0x0000: {
      if (opcode == op::Return) {
        ret();
        printf("returned to %x\n", isp);
        return true;
      }

      if (opcode == op::ClearDisplay) {
        printf("display cleared.");
        m_display->clear();
        return false;
      }
      break;
    }

    case op::Jump: {
      jmp(op::Address(opcode));
      printf("jumped to %x\n", isp);
      return true;
    }

    case op::Call: {
      call(op::Address(opcode));
      printf("called %x\n", isp);
      return true;
    }

    case op::SkipIfEqual: {
      printf("skipping if v%d (%x) == %x? ", op::CondReg(opcode),
             m_reg[op::CondReg(opcode)], op::CondReg(opcode));
      if ((m_reg[op::CondReg(opcode)]) == (op::CondVal(opcode))) {
        isp += 2;
        printf("skipped. \n");
        return true;
      }
      printf("nope. \n");
      return false;
    }

    case op::SkipIfNotEqual: {
      printf("skipping if v%d (%x) != %x? ", op::CondReg(opcode),
             m_reg[op::CondReg(opcode)], op::CondReg(opcode));

      if ((m_reg[op::CondReg(opcode)]) != (op::CondVal(opcode))) {
        isp += 2;
        printf("skipped. \n");
        return true;
      }
      printf("nope. \n");
      return false;
    }

    case op::SkipIfRegEqual: {
      auto reg1 = m_reg[op::LeftReg(opcode)];
      auto reg2 = m_reg[op::RightReg(opcode)];
      printf("skipping if v%d (%x) != v%d (%x)? ", op::LeftReg(opcode), reg1,
             op::RightReg(opcode), reg2);
      if (reg1 == reg2) {
        isp += 2;
        printf("skipped. \n");
        return true;
      }
      printf("nope. \n");
      return false;
    }

    case op::ConstSet: {
      m_reg[op::ConstSetReg(opcode)] = op::ConstSetVal(opcode);
      printf("set v%.1x = %x", op::ConstSetReg(opcode),
             op::ConstSetVal(opcode));
      return false;
    }

    case op::ConstAdd: {
      m_reg[op::ConstAddReg(opcode)] = op::ConstAddVal(opcode);
      return false;
    }

    case op::Math:
      return evalMathOp(opcode);

    case op::SkipIfRegNotEqual: {
      if (m_reg[op::LeftReg(opcode)] != m_reg[op::RightReg(opcode)]) {
        isp += 2;
        return true;
      }
      return false;
    }

    case op::SetAddressReg: {
      m_reg.i = op::Address(opcode);
      return false;
    }

    case op::OffsetJump: {
      jmp(m_reg.v0 + (op::Address(opcode)));
      return true;
    }

    case op::Random: {
      m_reg[op::LeftReg(opcode)] = random_byte() & op::ConstSetVal(opcode);
      return false;
    }

    case op::Draw: {
      auto x = m_reg[op::LeftReg(opcode)];
      auto y = m_reg[op::RightReg(opcode)];
      auto h = op::DrawHeight(opcode);
      uint8_t* start = &memory[m_reg.i];
      bool unset = false;
      for (uint16_t i = 0; i < h; i++) {
        unset |= m_display->drawByte(x, y + i, start[i]);
      }
      m_reg.vf = unset ? 1 : 0;
      m_display->redraw();
      return false;
    }

    case op::KeyCond: {
      auto key = m_reg[op::LeftReg(opcode)] & 0xF;
      if ((opcode & op::KeyCondEqMask) && m_keypad[key]) {
        isp += 2;
        return true;
      }
      if ((opcode & op::KeyCondNotEqMask) && !m_keypad[key]) {
        isp += 2;
        return true;
      }
      return false;
    }

    case op::Misc:
      return evalMiscOp(opcode);
  }
}

bool Chip8Emulator::exec() {
  bool ispSet = eval(fetch(isp));
  if (!ispSet) isp += 2;
  return ispSet;
}

}  // namespace chip8
