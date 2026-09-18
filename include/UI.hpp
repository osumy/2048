#pragma once

#include "Board.hpp"
#include "Leaderboard.hpp"
#include <string>
#include <vector>

namespace UI {
    int getTerminalWidth();
    std::string getIndent(int contentWidth);
    void clearScreen();
    void resetScreen();
    void renderMainMenu(char selected, bool showHelp);
    void renderBoard(const Board& board, int bestScore, const std::string& remainingTime);
    void renderHowToPlay();
    void renderWinnerPrompt();
    void renderGameOver();
    void renderLeaderboard(const std::vector<PlayerRecord>& records, int boardSize);
    int renderSizeSelection(const std::vector<int>& playedSizes);
}
