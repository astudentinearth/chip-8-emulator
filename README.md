
# CHIP-8 Emulator in Qt6 Widgets

## Building

```bash
cmake -B build
cmake --build build
```

`build/chip8-emulator` is the final output.

## Usage

```bash
chip8-emulator /path/to/file [--mocha|--kanagawa|--default]
```

The provided file must be a raw CHIP-8 ROM, with a maximum 3584 byte size (4K memory - 512 program loading offset). See references below to obtain some test ROMs.

`--mocha` applies Catppuccin colors to the viewport, while `--kanagawa` applies Kanagawa colors, and `--default` applies plain white-on-black colors.

## References

- [CHIP-8 Instruction Set](https://github.com/mattmikolay/chip-8/wiki/CHIP%E2%80%908-Instruction-Set)
- [CHIP-8 on Wikipedia](https://en.wikipedia.org/wiki/CHIP-8)
- [chip8-test-rom](https://github.com/corax89/chip8-test-rom) and [chip8-test-suite](https://github.com/Timendus/chip8-test-suite)

## Credits
- Beeping sound by [jeckkech on freesound.org](https://freesound.org/people/jeckkech/sounds/391650/) (CC0). `beep.pcm` was converted from the original `beep.wav` file.


## License

This program is free software under the GNU General Public License, either version 3 of the license, or any later version at your option.

See [LICENSE](./LICENSE) for more details.



