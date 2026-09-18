#pragma once

#include "Board.hpp"
#include "Leaderboard.hpp"
#include "Timer.hpp"
#include "UI.hpp"
#include <string>

class Game {
public:
    Game();

    void run();

private:
    void handleMainMenu();
    void startNewGame();
    void showLeaderboard();
    void preparePlayer();
    void playGame();

    Board m_board;
    Leaderboard m_leaderboard;
    GameTimer m_timer;
    std::string m_playerName;
    int m_boardSize{4};
    int m_bestScore{0};
};
