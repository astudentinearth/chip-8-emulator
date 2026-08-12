
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

void Chip8Display::clear() { printf("Display cleared!"); }

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
      jmp(opcode & op::Address);
      printf("jumped to %x\n", isp);
      return true;
    }

    case op::Call: {
      call(opcode & op::Address);
      printf("called %x\n", isp);
      return true;
    }

    case op::SkipIfEqual: {
      printf("skipping if v%d (%x) == %x? ", opcode & op::CondReg,
             m_reg[opcode & op::CondReg], opcode & op::CondReg);
      if ((m_reg[opcode & op::CondReg >> 8]) == (opcode & op::CondVal)) {
        isp += 2;
        printf("skipped. \n");
        return true;
      }
      printf("nope. \n");
      return false;
    }

    case op::SkipIfNotEqual: {
      printf("skipping if v%d (%x) != %x? ", opcode & op::CondReg,
             m_reg[opcode & op::CondReg], opcode & op::CondReg);

      if ((m_reg[opcode & op::CondReg]) != (opcode & op::CondVal)) {
        isp += 2;
        printf("skipped. \n");
        return true;
      }
      printf("nope. \n");
      return false;
    }

    case op::SkipIfRegEqual: {
      auto reg1 = m_reg[opcode & op::LeftReg];
      auto reg2 = m_reg[opcode & op::RightReg];
      printf("skipping if v%d (%x) != v%d (%x)? ", opcode & op::LeftReg, reg1,
             opcode & op::RightReg, reg2);
      if (reg1 == reg2) {
        isp += 2;
        printf("skipped. \n");
        return true;
      }
      printf("nope. \n");
      return false;
    }

    case op::ConstSet: {
      m_reg[opcode & op::ConstSetReg] = opcode & op::ConstSetVal;
      printf("set v%.1x = %x", opcode & op::ConstSetReg,
             opcode & op::ConstSetVal);
      return false;
    }

    case op::ConstAdd: {
      m_reg[opcode & op::ConstAddReg] = opcode & op::ConstAddVal;
      return false;
    }

    case op::Math: {
      switch (opcode & op::BinOpType) {
        case op::Assignment: {
          m_reg[opcode & op::LeftReg] = m_reg[opcode & op::RightReg];
          return false;
        }

        case op::Or: {
          m_reg[opcode & op::LeftReg] |= m_reg[opcode & op::RightReg];
          return false;
        }

        case op::And: {
          m_reg[opcode & op::LeftReg] &= m_reg[opcode & op::RightReg];
          return false;
        }

        case op::Xor: {
          m_reg[opcode & op::LeftReg] ^= m_reg[opcode & op::RightReg];
          return false;
        }

        case op::Add: {
          auto& left = m_reg[opcode & op::LeftReg];
          auto original = left;
          left += m_reg[opcode & op::RightReg];
          if (left < original)
            m_reg.vf = 1;
          else
            m_reg.vf = 0;
          return false;
        }

        case op::Sub: {
          auto& left = m_reg[opcode & op::LeftReg];
          auto original = left;
          left -= m_reg[opcode & op::RightReg];
          if (left > original)
            m_reg.vf = 0;
          else
            m_reg.vf = 1;
          return false;
        }

        case op::LeftShift: {
          auto& reg = m_reg[opcode & op::LeftReg];
          m_reg.vf = 0x8000 & reg;  // extract least significant bit
          reg <<= 1;
          return false;
        }

        case op::RightShift: {
          auto& reg = m_reg[opcode & op::LeftReg];
          m_reg.vf = 0x1 & reg;
          reg >>= 1;
          return false;
        }

        case op::Difference: {
          auto& vy = m_reg[opcode & op::RightReg];
          auto& vx = m_reg[opcode & op::LeftReg];
          auto result = vy - vx;
          if (vy >= vx)
            m_reg.vf = 1;
          else
            m_reg.vf = 0;
          vx = vy - vx;
          return false;
        }
      }

      case op::SkipIfRegNotEqual: {
        if (m_reg[opcode & op::LeftReg] != m_reg[opcode & op::RightReg]) {
          isp += 2;
          return true;
        }
        return false;
      }

      case op::SetAddressReg: {
        m_reg.i = opcode & op::Address;
        return false;
      }

      case op::OffsetJump: {
        jmp(m_reg.v0 + (opcode & op::Address));
        return true;
      }
    }
  }
  return false;
}

bool Chip8Emulator::exec() {
  bool ispSet = eval(fetch(isp));
  if (!ispSet) isp += 2;
  return ispSet;
}

}  // namespace chip8
