#include "Game.hpp"
#include "UI.hpp"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>

Game::Game()
    : m_board(4)
    , m_leaderboard("rec.txt") {
}

void Game::run() {
    char choice = 'n';
    bool showHelp = false;

    while (true) {
        UI::clearScreen();
        UI::renderMainMenu(choice, showHelp);

        char ch = _getch();
        switch (ch) {
            case 72:  // Up arrow
            case 'w':
            case 'W':
                choice = (choice == 'n') ? 'e' : (choice == 'l' ? 'n' : 'l');
                break;

            case 80:  // Down arrow
            case 's':
            case 'S':
                choice = (choice == 'n') ? 'l' : (choice == 'l' ? 'e' : 'n');
                break;

            case 13:  // Enter
            case 32:  // Space
                if (choice == 'e') {
                    return;
                } else if (choice == 'n') {
                    startNewGame();
                } else if (choice == 'l') {
                    showLeaderboard();
                }
                break;

            default:
                showHelp = true;
                break;
        }
    }
}

void Game::preparePlayer() {
    UI::clearScreen();

    while (true) {
        std::cout << "\u001b[93m >>\u001b[36m Please Enter your name: \u001b[96m";
        std::string name;
        std::getline(std::cin, name);
        if (!name.empty()) {
            m_playerName = name;
            break;
        }
        std::cout << "\u001b[91m INVALID!\n";
    }

    int n = 0;
    while (n < 2) {
        std::cout << "\n\u001b[93m >>\u001b[36m Please Enter the game board size: \u001b[96m";
        std::cin >> n;
        if (n < 2) {
            std::cout << "\u001b[91m INVALID!\n";
        }
    }
    m_boardSize = n;
    std::cin.ignore(10000, '\n'); // Clear trailing newline

    UI::renderHowToPlay();
    _getch();

    // Loading animation
    std::cout << "\n\n\u001b[92m Loading";
    for (int i = 0; i < 3; i++) {
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << ".";
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(300));
}

void Game::startNewGame() {
    preparePlayer();

    m_board.reset(m_boardSize);
    m_leaderboard.load();
    m_bestScore = m_leaderboard.getBestScore(m_boardSize);

    playGame();

    // Save player score
    m_leaderboard.addRecord(m_playerName, m_board.getScore(), m_boardSize);
}

void Game::playGame() {
    m_timer.start(5, 0);
    bool askedContinue = false;

    while (true) {
        UI::clearScreen();

        if (!askedContinue && m_board.getBiggestTile() >= 2048) {
            UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());
            UI::renderWinnerPrompt();
            char choose = _getch();
            if (choose == 'n' || choose == 'N') {
                m_timer.stop();
                return;
            }
            askedContinue = true;
            UI::clearScreen();
        }

        if (m_board.isFull() && !m_board.canMove()) {
            UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());
            UI::renderGameOver();
            _getch();
            m_timer.stop();
            return;
        }

        if (m_timer.isTimeUp()) {
            UI::clearScreen();
            std::cout << "\u001b[91m Game Over";
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            m_timer.stop();
            return;
        }

        UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());

        char move = _getch();
        switch (move) {
            case 'w':
            case 'W':
                m_board.move(Direction::Up);
                break;
            case 's':
            case 'S':
                m_board.move(Direction::Down);
                break;
            case 'a':
            case 'A':
                m_board.move(Direction::Left);
                break;
            case 'd':
            case 'D':
                m_board.move(Direction::Right);
                break;
            case 'b':
            case 'B':
                m_timer.stop();
                return;
        }
    }
}

void Game::showLeaderboard() {
    m_leaderboard.load();
    auto sizes = m_leaderboard.getPlayedSizes();
    if (sizes.empty()) {
        sizes = {4};
    }

    int selectedSize = UI::renderSizeSelection(sizes);
    auto records = m_leaderboard.getRecordsForSize(selectedSize);
    UI::renderLeaderboard(records, selectedSize);
    _getch();
}
