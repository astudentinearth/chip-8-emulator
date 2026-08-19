#include <SDL3/SDL.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#include "Window.hpp"
#include "chip8.hpp"

using namespace std;
using namespace chip8;

void printIsp(const Chip8Emulator* emulator) {
  printf("isp: 0x%.2X | opcode: 0x%.2X\n", emulator->getIsp(),
         emulator->fetch(emulator->getIsp()));
}

void runSDLApp(Framebuffer& fb) {
  SDL_Init(SDL_INIT_VIDEO);
  auto window = new EmulatorWindow(fb);
  bool running = true;
  while (running) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (event.type == SDL_EVENT_QUIT) {
        running = false;
      }
    }

    window->draw();
  }
  delete window;
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

  const auto display = Chip8Display::create([](const Framebuffer& buf) {});
  const auto emulator = Chip8Emulator::create(display.get());
  if (emulator->loadProgram(buffer.get(), size)) {
    cout << "Loaded program successfully." << endl;
  } else {
    cerr << "Unexpected error: failed to load program." << endl;
    return 1;
  }

  cout << "Creating SDL window" << endl;
  thread t([&display]() { runSDLApp(display->getFramebuffer()); });
  cout << "Running program" << endl;
  emulator->run();
  t.join();
  return 0;
}
