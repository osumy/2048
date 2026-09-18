#pragma once

#include <string>
#include <cstdint>
#include <sstream>

namespace Theme {

struct Color {
    uint8_t r{0};
    uint8_t g{0};
    uint8_t b{0};
};

// Common UI Colors (Retro Arcade / Neon Synthwave)
constexpr Color BG_DARK      { 18,  20,  32}; // Deep space navy
constexpr Color BG_CARD      { 28,  31,  48}; // Card / box background
constexpr Color BG_CELL_EMPTY{ 36,  40,  62}; // Empty tile background
constexpr Color FG_MUTED     {100, 110, 145}; // Muted text / borders
constexpr Color FG_BRIGHT    {240, 245, 255}; // Crisp white text
constexpr Color NEON_CYAN    {  0, 240, 255}; // Electric cyan
constexpr Color NEON_PINK    {255,  42, 141}; // Laser magenta/pink
constexpr Color NEON_AMBER   {255, 183,   3}; // Golden glow
constexpr Color NEON_GREEN   {  6, 214, 160}; // Radiant emerald
constexpr Color NEON_PURPLE  {147,  51, 234}; // Mystic purple
constexpr Color NEON_RED     {239,  68,  68}; // Alert red

struct TileStyle {
    Color fg;
    Color bg;
    bool bold{true};
};

inline std::string fg(Color c) {
    return "\x1b[38;2;" + std::to_string(c.r) + ";" +
                          std::to_string(c.g) + ";" +
                          std::to_string(c.b) + "m";
}

inline std::string bg(Color c) {
    return "\x1b[48;2;" + std::to_string(c.r) + ";" +
                          std::to_string(c.g) + ";" +
                          std::to_string(c.b) + "m";
}

inline std::string reset() {
    return "\x1b[0m";
}

inline std::string bold() {
    return "\x1b[1m";
}

inline std::string dim() {
    return "\x1b[2m";
}

inline std::string hideCursor() {
    return "\x1b[?25l";
}

inline std::string showCursor() {
    return "\x1b[?25h";
}

inline std::string homeCursor() {
    return "\x1b[H";
}

inline std::string clearAll() {
    return "\x1b[2J\x1b[H";
}

inline TileStyle getTileStyle(int val) {
    switch (val) {
        case 0:
            return {{60, 68, 98}, BG_CELL_EMPTY, false};
        case 2:
            return {{40, 36, 42}, {236, 224, 209}, true}; // Soft warm ivory
        case 4:
            return {{36, 30, 25}, {235, 213, 179}, true}; // Amber cream
        case 8:
            return {{255, 255, 255}, {245, 133, 73}, true}; // Arcade tangerine
        case 16:
            return {{255, 255, 255}, {238, 93, 58}, true}; // Sunset flame
        case 32:
            return {{255, 255, 255}, {230, 57, 70}, true}; // Laser crimson
        case 64:
            return {{255, 255, 255}, {217, 4, 41}, true}; // Hyper red
        case 128:
            return {{20, 20, 30}, {255, 209, 102}, true}; // Cyber yellow
        case 256:
            return {{10, 30, 20}, {6, 214, 160}, true}; // Neon emerald
        case 512:
            return {{4, 20, 35}, {0, 212, 255}, true}; // Neon cyan
        case 1024:
            return {{255, 255, 255}, {131, 56, 236}, true}; // Electric violet
        case 2048:
            return {{15, 15, 20}, {255, 190, 11}, true}; // Legendary golden sun
        case 4096:
            return {{255, 255, 255}, {247, 37, 133}, true}; // Neon hot magenta
        default:
            return {{255, 255, 255}, {76, 201, 240}, true}; // Supernova cyan-blue
    }
}

} // namespace Theme
