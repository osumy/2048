#include "UI.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>
#include <cstdlib>

namespace UI {

void clearScreen() {
    std::system("cls");
}

static int digitCount(int number) {
    if (number == 0) return 1;
    int digs = 0;
    while (number > 0) {
        number /= 10;
        digs++;
    }
    return digs;
}

static void printMenuLine() {
    std::cout << "\u001b[46m\u001b[93m\u2551";
    for (int i = 0; i < 68; i++) std::cout << " ";
    std::cout << "\u2551\u001b[0m\n";
}

static void printButtonUpDown(bool isUp, bool isOn) {
    std::cout << "\u001b[46m\u001b[93m\u2551";
    for (int i = 0; i < 26; i++) std::cout << " ";

    std::cout << "\u001b[97m";
    if (isOn) std::cout << "\u001b[106m";
    std::cout << (isUp ? "\u250F" : "\u2517");

    for (int i = 0; i < 14; i++) std::cout << "\u2501";

    if (isUp) {
        std::cout << "\u2513\u001b[46m\u001b[93m";
    } else {
        std::cout << "\u251B\u001b[46m\u001b[93m";
    }

    for (int i = 0; i < 26; i++) std::cout << " ";
    std::cout << "\u2551\u001b[0m\n";
}

static void printMenuButton(const std::string& str, bool isOn) {
    int space = (14 - static_cast<int>(str.length())) / 2;

    printButtonUpDown(true, isOn);

    std::cout << "\u001b[46m\u001b[93m\u2551";
    if (isOn) {
        for (int i = 0; i < 24; i++) std::cout << " ";
        std::cout << ">>";
    } else {
        for (int i = 0; i < 26; i++) std::cout << " ";
    }

    std::cout << "\u001b[97m";
    if (isOn) std::cout << "\u001b[106m";
    std::cout << "\u2502";

    for (int i = 0; i < space; i++) std::cout << " ";
    std::cout << str;
    for (int i = 0; i < space; i++) std::cout << " ";
    std::cout << "\u2502\u001b[93m\u001b[46m";

    if (isOn) {
        std::cout << "<<";
        for (int i = 0; i < 24; i++) std::cout << " ";
    } else {
        for (int i = 0; i < 26; i++) std::cout << " ";
    }
    std::cout << "\u2551\u001b[0m\n";

    printButtonUpDown(false, isOn);
}

void renderMainMenu(char selected, bool showHelp) {
    std::cout << "\u001b[46m\u001b[93m\u2554";
    for (int i = 0; i < 68; i++) std::cout << "\u2550";
    std::cout << "\u2557\u001b[0m\n";

    printMenuLine();
    std::cout << "\u001b[46m\u001b[93m\u2551            _______   ________   ___   ___   ________               \u2551\u001b[0m\n";
    std::cout << "\u001b[46m\u001b[93m\u2551           /  ___  \\\\|\\   __  \\ |\\  \\ |\\  \\ |\\   __  \\              \u2551\u001b[0m\n";
    std::cout << "\u001b[46m\u001b[93m\u2551          /__/|_/  //\\ \\  \\|\\  \\\\ \\  \\\\_\\  \\\\ \\  \\|\\  \\             \u2551\u001b[0m\n";
    std::cout << "\u001b[46m\u001b[93m\u2551          |__|//  //  \\ \\  \\\\\\  \\\\ \\______  \\\\ \\   __  \\            \u2551\u001b[0m\n";
    std::cout << "\u001b[46m\u001b[93m\u2551              /  //___ \\ \\  \\\\\\  \\\\|_____|\\  \\\\ \\  \\|\\  \\           \u2551\u001b[0m\n";
    std::cout << "\u001b[46m\u001b[93m\u2551             |\\________\\\\ \\_______\\      \\ \\__\\\\ \\_______\\          \u2551\u001b[0m\n";
    std::cout << "\u001b[46m\u001b[93m\u2551              \\|_______| \\|_______|       \\|__| \\|_______|          \u2551\u001b[0m\n";
    printMenuLine();
    printMenuLine();

    printMenuButton("New Game", selected == 'n');
    printMenuLine();
    printMenuButton("Leader Board", selected == 'l');
    printMenuLine();
    printMenuButton("Exit", selected == 'e');
    printMenuLine();

    if (showHelp) {
        std::cout << "\u001b[46m\u001b[93m\u2551 \u001b[97m Use w s  or  arrow up  arrow down\u001b[46m\u001b[93m";
        for (int i = 0; i < 33; i++) std::cout << " ";
        std::cout << "\u2551\u001b[0m\n";
    }

    std::cout << "\u001b[46m\u001b[93m\u255A";
    for (int i = 0; i < 68; i++) std::cout << "\u2550";
    std::cout << "\u255D\u001b[0m\n";
}

void renderBoard(const Board& board, int bestScore, const std::string& remainingTime) {
    // Score box
    std::cout << "\u001b[36m";
    for (int i = 0; i < 23; i++) std::cout << "\u2550";
    std::cout << "\nScore: \u001b[96m" << board.getScore() << "\n";
    std::cout << "\u001b[36mBest Score: \u001b[96m" << bestScore << "\n";
    std::cout << "\u001b[36m";
    for (int i = 0; i < 23; i++) std::cout << "\u2550";
    std::cout << "\n\n\n";

    int n = board.getSize();
    int len = digitCount(board.getBiggestTile());

    auto printLenLine = [len]() {
        for (int j = 0; j < len + 2; j++) std::cout << "\u2501";
    };

    // First line
    for (int i = 0; i < n; i++) {
        std::cout << (i == 0 ? "\u250F" : "\u2533");
        printLenLine();
    }
    std::cout << "\u2513\n";

    // Rows
    for (int i = 0; i < n; i++) {
        if (i > 0) {
            for (int j = 0; j < n; j++) {
                std::cout << (j == 0 ? "\u2523" : "\u254B");
                printLenLine();
            }
            std::cout << "\u252B\n";
        }

        for (int j = 0; j < n; j++) {
            std::cout << "\u2503";
            int val = board.getCell(i, j);
            int d = digitCount(val);
            double tmp = (len - d) / 2.0;
            std::cout << " ";
            for (int k = 0; k < std::floor(tmp); k++) std::cout << " ";
            std::cout << "\u001b[96m";
            if (val != 0) std::cout << val;
            else std::cout << " ";
            std::cout << "\u001b[36m";
            for (int k = 0; k < std::ceil(tmp); k++) std::cout << " ";
            std::cout << " ";
        }
        std::cout << "\u2503\n";
    }

    // Last line
    for (int i = 0; i < n; i++) {
        std::cout << (i == 0 ? "\u2517" : "\u253B");
        printLenLine();
    }
    std::cout << "\u251B\n";

    std::cout << "\n " << remainingTime << "\n\n  ";
}

void renderHowToPlay() {
    clearScreen();
    std::cout << "\u001b[36mHow to play?\n";
    std::cout << "--------------------------------------------------------------\n";
    std::cout << "\u001b[96mYou will see a nxn screen with some numbers in some houses.\n";
    std::cout << "In each turn, you can move up, down, left and right with the wasd keys.\n";
    std::cout << "The game will continue until it is no longer possible to move!\n\n";
    std::cout << "(Press b to return to the menu)\n\n";
    std::cout << "Press any key to continue...";
}

void renderWinnerPrompt() {
    std::cout << "\n\u001b[92m Winner!\n\n";
    std::cout << "\u001b[93m >>\u001b[36m Do you want to continue playing? (y/n)\u001b[96m\n >> ";
}

void renderGameOver() {
    std::cout << "\n\u001b[91m Game Over\n\n";
    std::cout << "\u001b[93m >>\u001b[36m press any key to continue...";
}

int renderSizeSelection(const std::vector<int>& playedSizes) {
    clearScreen();
    std::cout << "\u001b[36mLeader Board\n";
    for (int i = 0; i < 23; i++) std::cout << "\u2550";
    std::cout << "\n\n\u001b[93m >>\u001b[36m Please select game board size\n";

    for (int size : playedSizes) {
        std::cout << "\u001b[93m     >>\u001b[36m " << size << " x " << size << "\n";
    }
    std::cout << "\n\u001b[93m >> ";

    std::string ch;
    std::cin >> ch;
    try {
        return std::stoi(ch);
    } catch (...) {
        return 4;
    }
}

void renderLeaderboard(const std::vector<PlayerRecord>& records, int boardSize) {
    clearScreen();
    std::cout << "\u001b[93m Name";
    for (int i = 0; i < 46; i++) std::cout << " ";
    std::cout << "Score";
    for (int i = 0; i < 15; i++) std::cout << " ";
    std::cout << "n x n\n";

    for (const auto& rec : records) {
        std::cout << "\u001b[96m " << rec.name;
        int len = static_cast<int>(rec.name.length());
        for (int k = 0; k < 50 - len; k++) std::cout << " ";
        std::cout << rec.score;
        len = static_cast<int>(std::to_string(rec.score).length());
        for (int k = 0; k < 20 - len; k++) std::cout << " ";
        std::cout << boardSize << " x " << boardSize << "\n";
    }

    std::cout << "\nPress any key to continue...";
}

} // namespace UI
