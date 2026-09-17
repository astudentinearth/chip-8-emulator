#include <SDL3/SDL.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#include "App.hpp"
#include "Color.hpp"
#include "chip8.hpp"

using namespace std;
using namespace chip8;

int main(int argc, const char **argv) {
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

  const auto display = Chip8Display::create([](const Framebuffer &buf) {});
  const shared_ptr<Chip8Emulator> emulator =
      std::move(Chip8Emulator::create(display.get()));
  if (emulator->loadProgram(buffer.get(), size)) {
    cout << "Loaded program successfully." << endl;
  } else {
    cerr << "Unexpected error: failed to load program." << endl;
    return 1;
  }

  ColorScheme theme = ColorScheme::Default;

  for (size_t i = 1; i < argc; i++) {
    auto arg = std::string(argv[i]);
    if (arg == "--kanagawa")
      theme = ColorScheme::Kanagawa;
    if (arg == "--mocha")
      theme = ColorScheme::Mocha;
    if (arg == "--default-colors")
      theme = ColorScheme::Default;
  }

  cout << "Creating SDL window" << endl;

  auto context = AppContext{
      .filename = filename,
      .colorscheme = theme,
      .emulator = emulator,
      .framebuffer = display->getFramebuffer(),
  };

  thread t([&context]() { runSDLApp(context); });
  cout << "Running program" << endl;
  emulator->run();
  t.join();
  return 0;
}
