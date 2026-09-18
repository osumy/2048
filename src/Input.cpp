#include "Input.hpp"

#ifdef _WIN32
#include <conio.h>

namespace Input {

Event readKey() {
    int ch = _getch();
    if (ch == 0 || ch == 224 || ch == -32) {
        int code = _getch();
        switch (code) {
            case 72: return {Key::Up, 0};
            case 80: return {Key::Down, 0};
            case 75: return {Key::Left, 0};
            case 77: return {Key::Right, 0};
            default: return {Key::None, 0};
        }
    }

    if (ch == 13 || ch == 10) return {Key::Enter, '\n'};
    if (ch == 32) return {Key::Space, ' '};

    return {Key::Char, static_cast<char>(ch)};
}

void waitAnyKey() {
    int ch = _getch();
    if (ch == 0 || ch == 224 || ch == -32) {
        _getch();
    }
}

} // namespace Input

#else
#include <termios.h>
#include <unistd.h>
#include <cstdio>

namespace Input {

static int getchRaw() {
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    int ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}

Event readKey() {
    int ch = getchRaw();
    if (ch == 27) { // ANSI escape sequence
        int c2 = getchRaw();
        if (c2 == 91) { // '['
            int c3 = getchRaw();
            switch (c3) {
                case 'A': return {Key::Up, 0};
                case 'B': return {Key::Down, 0};
                case 'C': return {Key::Right, 0};
                case 'D': return {Key::Left, 0};
                default: return {Key::None, 0};
            }
        }
        return {Key::None, 0};
    }

    if (ch == 10 || ch == 13) return {Key::Enter, '\n'};
    if (ch == 32) return {Key::Space, ' '};

    return {Key::Char, static_cast<char>(ch)};
}

void waitAnyKey() {
    int ch = getchRaw();
    if (ch == 27) {
        getchRaw();
        getchRaw();
    }
}

} // namespace Input
#endif
