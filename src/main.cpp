#include "Game.hpp"
#include "Theme.hpp"
#include <ctime>
#include <cstdlib>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#endif

static void setupTerminal() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            SetConsoleMode(hOut, dwMode);
        }
    }
    SetConsoleTitleA("2048 - Retro Arcade Edition");
#endif
    std::cout << Theme::hideCursor();
}

static void restoreTerminal() {
    std::cout << Theme::showCursor() << Theme::reset() << std::endl;
}

int main() {
    setupTerminal();
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    Game game;
    game.run();

    restoreTerminal();
    return 0;
}

