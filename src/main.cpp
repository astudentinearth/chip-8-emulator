#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>

#include "chip8.hpp"

using namespace std;
using namespace chip8;

void printIsp(const Chip8Emulator* emulator) {
  printf("isp: 0x%.2X | opcode: 0x%.2X\n", emulator->getIsp(),
         emulator->fetch(emulator->getIsp()));
}

int main(int argc, const char** argv) {
  if (argc < 2) {
    cerr << "Error: no filename provided." << endl;
    return 1;
  }

  const string filename(argv[1]);
  ifstream programFile(filename, ios_base::binary);

  if (!programFile.good()) {
    cerr << "Error: cannot read file: \"" << filename << '"' << endl;
    return 1;
  }

  const auto size = filesystem::file_size(filename);

  if (size > Chip8Emulator::MaxProgramSize) {
    cerr << "Error: this file won't fit into memory (" << size << " bytes)"
         << " | Max allowed: " << Chip8Emulator::MaxProgramSize << " bytes"
         << endl;
    return 1;
  }

  auto buffer = make_unique<char[]>(size);
  programFile.read(buffer.get(), size);

  const auto display = Chip8Display::create([](const Framebuffer& buf) {
    for (int i = 0; i < CHIP8_DISPLAY_HEIGHT * CHIP8_DISPLAY_WIDTH; i++) {
        auto byte = buf[i];
        for(int i = 0; i < 8; i++) {
            cout << (((byte << i) & 0x80) == 0 ? ' ' : '@');
        }
        cout << endl;
    }
  });
  const auto emulator = Chip8Emulator::create(display.get());
  if (emulator->loadProgram(buffer.get(), size)) {
    cout << "Loaded program successfully." << endl;
  } else {
    cerr << "Unexpected error: failed to load program." << endl;
    return 1;
  }

  while (true) {
    string input{};
    cout << "> ";
    if (!(cin >> input)) break;
    if (input == "next") {
      emulator->fetchNext();
      printIsp(emulator.get());
    }
    if (input == "isp") {
      printIsp(emulator.get());
    }
    if (input == "reg") emulator->dumpState();
    if (input == "exec") {
      emulator->exec();
      printIsp(emulator.get());
    }
    if (input == "exit") break;
  }

  return 0;
}
