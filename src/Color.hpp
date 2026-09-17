
#pragma once
#include <cstdint>

struct Color {
  std::uint8_t red, green, blue, alpha;

  static constexpr std::uint8_t extractRed(std::uint32_t rgba) {
    return (rgba >> 24) & 0xFF;
  }
  static constexpr std::uint8_t extractGreen(std::uint32_t rgba) {
    return (rgba >> 16) & 0xFF;
  }
  static constexpr std::uint8_t extractBlue(std::uint32_t rgba) {
    return (rgba >> 8) & 0xFF;
  }
  static constexpr std::uint8_t extractAlpha(std::uint32_t rgba) {
    return rgba & 0xFF;
  }

  static constexpr Color rgba(std::uint32_t rgba) {
    return {.red = extractRed(rgba),
            .green = extractGreen(rgba),
            .blue = extractBlue(rgba),
            .alpha = extractAlpha(rgba)};
  }

  static const Color White;
  static const Color Black;
};

constexpr Color Color::White{255, 255, 255, 255};
constexpr Color Color::Black{0, 0, 0, 255};

struct ColorScheme {
  Color background;
  Color foreground;

  static const ColorScheme Default;
  static const ColorScheme Kanagawa;
  static const ColorScheme Mocha;
};

constexpr ColorScheme ColorScheme::Default{Color::Black, Color::White};
constexpr ColorScheme ColorScheme::Kanagawa{Color::rgba(0x1F1F28FF),
                                            Color::rgba(0xDCD7BAFF)};
constexpr ColorScheme ColorScheme::Mocha{Color::rgba(0x1E1E2EFF),
                                         Color::rgba(0xCDD6F4FF)};
