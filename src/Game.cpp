#include "Game.hpp"
#include "UI.hpp"
#include "Theme.hpp"
#include "Audio.hpp"
#include <iostream>
#include <conio.h>
#include <chrono>
#include <thread>

Game::Game()
    : m_board(4)
    , m_leaderboard("rec.txt") {
}

void Game::run() {
    UI::resetScreen();
    char choice = 'n';
    bool showHelp = false;

    while (true) {
        UI::clearScreen();
        UI::renderMainMenu(choice, showHelp);

        int ch = _getch();
        bool isExtended = false;
        if (ch == 0 || ch == 224 || ch == -32) {
            isExtended = true;
            ch = _getch();
        }

        if (isExtended) {
            switch (ch) {
                case 72:  // Up arrow
                    choice = (choice == 'n') ? 'e' : (choice == 'l' ? 'n' : 'l');
                    break;
                case 80:  // Down arrow
                    choice = (choice == 'n') ? 'l' : (choice == 'l' ? 'e' : 'n');
                    break;
            }
        } else {
            switch (ch) {
                case 'w':
                case 'W':
                    choice = (choice == 'n') ? 'e' : (choice == 'l' ? 'n' : 'l');
                    break;

                case 's':
                case 'S':
                    choice = (choice == 'n') ? 'l' : (choice == 'l' ? 'e' : 'n');
                    break;

                case 13:  // Enter
                case 32:  // Space
                    if (choice == 'e') {
                        UI::resetScreen();
                        return;
                    } else if (choice == 'n') {
                        startNewGame();
                        UI::resetScreen();
                    } else if (choice == 'l') {
                        showLeaderboard();
                        UI::resetScreen();
                    }
                    break;

                default:
                    showHelp = true;
                    break;
            }
        }
    }
}

void Game::preparePlayer() {
    UI::resetScreen();

    std::cout << "\n  " << Theme::fg(Theme::NEON_CYAN) << "╭─────────────────── PLAYER REGISTRATION ──────────────────╮" << Theme::reset() << "\n";
    std::cout << "  " << Theme::fg(Theme::NEON_CYAN) << "│" << Theme::reset()
              << Theme::fg(Theme::NEON_AMBER) << Theme::bold()
              << "               ENTER YOUR ARCADE TAG / NAME               "
              << Theme::reset() << Theme::fg(Theme::NEON_CYAN) << "│" << Theme::reset() << "\n";
    std::cout << "  " << Theme::fg(Theme::NEON_CYAN) << "╰──────────────────────────────────────────────────────────╯" << Theme::reset() << "\n\n";

    while (true) {
        std::cout << "  " << Theme::fg(Theme::NEON_PINK) << "▶ "
                  << Theme::fg(Theme::FG_BRIGHT) << "Player Name: " << Theme::fg(Theme::NEON_CYAN);
        std::string name;
        std::getline(std::cin, name);
        std::cout << Theme::reset();
        if (!name.empty()) {
            m_playerName = name;
            break;
        }
        std::cout << "  " << Theme::fg(Theme::NEON_RED) << "⚠ Name cannot be empty! Please try again.\n" << Theme::reset();
    }

    int n = 0;
    while (n < 2 || n > 8) {
        std::cout << "\n  " << Theme::fg(Theme::NEON_PINK) << "▶ "
                  << Theme::fg(Theme::FG_BRIGHT) << "Enter Board Size (2 - 8, default 4): " << Theme::fg(Theme::NEON_CYAN);
        std::string line;
        std::getline(std::cin, line);
        std::cout << Theme::reset();
        if (line.empty()) {
            n = 4;
            break;
        }
        try {
            n = std::stoi(line);
        } catch (...) {
            n = 0;
        }
        if (n < 2 || n > 8) {
            std::cout << "  " << Theme::fg(Theme::NEON_RED) << "⚠ Invalid size! Please enter a number between 2 and 8.\n" << Theme::reset();
        }
    }
    m_boardSize = n;

    UI::renderHowToPlay();
    _getch();

    UI::resetScreen();
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
    UI::resetScreen();
    m_timer.start(5, 0);
    Audio::startBGM();
    bool askedContinue = false;

    while (true) {
        UI::clearScreen();

        if (!askedContinue && m_board.getBiggestTile() >= 2048) {
            Audio::playSFX(Audio::SFX::Win);
            UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());
            UI::renderWinnerPrompt();
            char choose = _getch();
            if (choose == 'n' || choose == 'N') {
                m_timer.stop();
                Audio::stopBGM();
                return;
            }
            askedContinue = true;
            UI::resetScreen();
        }

        if (m_board.isFull() && !m_board.canMove()) {
            Audio::stopBGM();
            Audio::playSFX(Audio::SFX::GameOver);
            UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());
            UI::renderGameOver();
            _getch();
            m_timer.stop();
            return;
        }

        if (m_timer.isTimeUp()) {
            Audio::stopBGM();
            Audio::playSFX(Audio::SFX::GameOver);
            UI::clearScreen();
            std::cout << "\n  " << Theme::fg(Theme::NEON_RED) << Theme::bold()
                      << "⌛ TIME EXPIRED! GAME OVER" << Theme::reset() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1200));
            m_timer.stop();
            return;
        }

        UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());

        int move = _getch();
        bool isExtended = false;
        if (move == 0 || move == 224 || move == -32) {
            isExtended = true;
            move = _getch();
        }

        int oldScore = m_board.getScore();
        bool moved = false;

        if (isExtended) {
            switch (move) {
                case 72:  // Up arrow
                    moved = m_board.move(Direction::Up);
                    break;
                case 80:  // Down arrow
                    moved = m_board.move(Direction::Down);
                    break;
                case 75:  // Left arrow
                    moved = m_board.move(Direction::Left);
                    break;
                case 77:  // Right arrow
                    moved = m_board.move(Direction::Right);
                    break;
            }
        } else {
            switch (move) {
                case 'w':
                case 'W':
                    moved = m_board.move(Direction::Up);
                    break;
                case 's':
                case 'S':
                    moved = m_board.move(Direction::Down);
                    break;
                case 'a':
                case 'A':
                    moved = m_board.move(Direction::Left);
                    break;
                case 'd':
                case 'D':
                    moved = m_board.move(Direction::Right);
                    break;
                case 'm':
                case 'M':
                    Audio::toggleBGM();
                    break;
                case 'b':
                case 'B':
                    m_timer.stop();
                    Audio::stopBGM();
                    return;
            }
        }

        if (moved) {
            if (m_board.getScore() > oldScore) {
                Audio::playSFX(Audio::SFX::Merge);
            } else {
                Audio::playSFX(Audio::SFX::Move);
            }
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
