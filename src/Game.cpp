#include "Game.hpp"
#include "UI.hpp"
#include "Theme.hpp"
#include "Audio.hpp"
#include "Input.hpp"
#include <iostream>
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

        Input::Event ev = Input::readKey();
        if (ev.key == Input::Key::Up || (ev.key == Input::Key::Char && (ev.ch == 'w' || ev.ch == 'W'))) {
            choice = (choice == 'n') ? 'e' : (choice == 'l' ? 'n' : 'l');
        } else if (ev.key == Input::Key::Down || (ev.key == Input::Key::Char && (ev.ch == 's' || ev.ch == 'S'))) {
            choice = (choice == 'n') ? 'l' : (choice == 'l' ? 'e' : 'n');
        } else if (ev.key == Input::Key::Char && (ev.ch == 'm' || ev.ch == 'M')) {
            Audio::toggleBGM();
        } else if (ev.key == Input::Key::Enter || ev.key == Input::Key::Space) {
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
        } else {
            showHelp = true;
        }
    }
}

void Game::preparePlayer() {
    UI::resetScreen();
    std::string ind = UI::getIndent(60);

    std::cout << "\n" << ind << Theme::fg(Theme::NEON_CYAN) << "╭─────────────────── PLAYER REGISTRATION ──────────────────╮" << Theme::reset() << "\n";
    std::cout << ind << Theme::fg(Theme::NEON_CYAN) << "│" << Theme::reset()
              << Theme::fg(Theme::NEON_AMBER) << Theme::bold()
              << "                  ENTER YOUR PLAYER NAME                  "
              << Theme::reset() << Theme::fg(Theme::NEON_CYAN) << "│" << Theme::reset() << "\n";
    std::cout << ind << Theme::fg(Theme::NEON_CYAN) << "╰──────────────────────────────────────────────────────────╯" << Theme::reset() << "\n\n";

    while (true) {
        std::cout << ind << Theme::fg(Theme::NEON_PINK) << "▶ "
                  << Theme::fg(Theme::FG_BRIGHT) << "Player Name: " << Theme::fg(Theme::NEON_CYAN);
        std::string name;
        std::getline(std::cin, name);
        std::cout << Theme::reset();

        size_t start = name.find_first_not_of(" \t\r\n");
        size_t end = name.find_last_not_of(" \t\r\n");
        if (start != std::string::npos && end != std::string::npos) {
            m_playerName = name.substr(start, end - start + 1);
            break;
        }
        std::cout << ind << Theme::fg(Theme::NEON_RED) << "[!] Name cannot be empty! Please try again.\n" << Theme::reset();
    }

    int n = 0;
    while (n < 2 || n > 8) {
        std::cout << "\n" << ind << Theme::fg(Theme::NEON_PINK) << "▶ "
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
            std::cout << ind << Theme::fg(Theme::NEON_RED) << "[!] Invalid size! Please enter a number between 2 and 8.\n" << Theme::reset();
        }
    }
    m_boardSize = n;

    UI::renderHowToPlay();
    Input::waitAnyKey();

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
    bool askedContinue = false;

    while (true) {
        UI::clearScreen();

        if (!askedContinue && m_board.getBiggestTile() >= 2048) {
            Audio::playSFX(Audio::SFX::Win);
            UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());
            UI::renderWinnerPrompt();
            Input::Event choose = Input::readKey();
            if (choose.key == Input::Key::Char && (choose.ch == 'n' || choose.ch == 'N')) {
                m_timer.stop();
                return;
            }
            askedContinue = true;
            UI::resetScreen();
        }

        if (m_board.isFull() && !m_board.canMove()) {
            Audio::playSFX(Audio::SFX::GameOver);
            UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());
            UI::renderGameOver();
            Input::waitAnyKey();
            m_timer.stop();
            return;
        }

        if (m_timer.isTimeUp()) {
            Audio::playSFX(Audio::SFX::GameOver);
            UI::clearScreen();
            std::string ind = UI::getIndent(26);
            std::cout << "\n" << ind << Theme::fg(Theme::NEON_RED) << Theme::bold()
                      << "⌛ TIME EXPIRED! GAME OVER" << Theme::reset() << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(1200));
            m_timer.stop();
            return;
        }

        UI::renderBoard(m_board, m_bestScore, m_timer.getRemainingFormatted());

        Input::Event move = Input::readKey();
        int oldScore = m_board.getScore();
        bool moved = false;

        if (move.key == Input::Key::Up || (move.key == Input::Key::Char && (move.ch == 'w' || move.ch == 'W'))) {
            moved = m_board.move(Direction::Up);
        } else if (move.key == Input::Key::Down || (move.key == Input::Key::Char && (move.ch == 's' || move.ch == 'S'))) {
            moved = m_board.move(Direction::Down);
        } else if (move.key == Input::Key::Left || (move.key == Input::Key::Char && (move.ch == 'a' || move.ch == 'A'))) {
            moved = m_board.move(Direction::Left);
        } else if (move.key == Input::Key::Right || (move.key == Input::Key::Char && (move.ch == 'd' || move.ch == 'D'))) {
            moved = m_board.move(Direction::Right);
        } else if (move.key == Input::Key::Char && (move.ch == 'm' || move.ch == 'M')) {
            Audio::toggleBGM();
        } else if (move.key == Input::Key::Char && (move.ch == 'b' || move.ch == 'B')) {
            m_timer.stop();
            return;
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
    Input::waitAnyKey();
}
