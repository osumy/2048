#include "UI.hpp"
#include "Theme.hpp"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <string>

namespace UI {

void clearScreen() {
    std::cout << Theme::homeCursor();
}

void resetScreen() {
    std::cout << Theme::clearAll();
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

static std::string padCenter(const std::string& str, int width) {
    int len = static_cast<int>(str.length());
    if (len >= width) return str;
    int left = (width - len) / 2;
    int right = width - len - left;
    return std::string(left, ' ') + str + std::string(right, ' ');
}

static std::string formatNumber(int val) {
    std::string s = std::to_string(val);
    int n = static_cast<int>(s.length());
    if (n <= 3) return s;
    std::string res;
    int count = 0;
    for (int i = n - 1; i >= 0; --i) {
        res.push_back(s[i]);
        count++;
        if (count == 3 && i > 0) {
            res.push_back(',');
            count = 0;
        }
    }
    std::reverse(res.begin(), res.end());
    return res;
}

void renderMainMenu(char selected, bool showHelp) {
    std::ostringstream out;
    out << Theme::homeCursor();

    const std::string bannerLine1 = "  ██████╗   ██████╗  ██╗  ██╗  ██████╗ ";
    const std::string bannerLine2 = " ╚════██╗  ██╔═████╗ ██║  ██║ ██╔════╝ ";
    const std::string bannerLine3 = "  █████╔╝  ██║██╔██║ ███████║ ╚█████╗  ";
    const std::string bannerLine4 = " ██╔═══╝   ████╔╝██║ ╚════██║  ╚═══██╗ ";
    const std::string bannerLine5 = " ███████╗  ╚██████╔╝      ██║ ██████╔╝ ";
    const std::string bannerLine6 = " ╚══════╝   ╚═════╝       ╚═╝ ╚═════╝  ";

    auto printBanner = [&](const std::string& line, Theme::Color col) {
        out << "   " << Theme::fg(col) << Theme::bold() << line << Theme::reset() << "\n";
    };

    out << "\n";
    printBanner(bannerLine1, {0, 240, 255});
    printBanner(bannerLine2, {40, 210, 255});
    printBanner(bannerLine3, {120, 160, 255});
    printBanner(bannerLine4, {190, 90, 255});
    printBanner(bannerLine5, {255, 42, 180});
    printBanner(bannerLine6, {255, 42, 141});

    out << "\n";
    out << "      " << Theme::fg(Theme::NEON_AMBER) << Theme::bold()
        << "[  RETRO ARCADE CHIPTUNE EDITION  ]" << Theme::reset() << "\n\n";

    auto renderButton = [&](const std::string& label, bool isSelected) {
        int btnWidth = 34;
        if (isSelected) {
            out << "     " << Theme::fg(Theme::NEON_PINK) << "╭"
                << std::string(btnWidth + 2, '-') << "╮" << Theme::reset() << "\n";
            out << "     " << Theme::fg(Theme::NEON_PINK) << "│ "
                << Theme::bg(Theme::BG_CARD) << Theme::fg(Theme::NEON_CYAN) << Theme::bold()
                << "▶ " << padCenter(label, btnWidth - 4) << " ◀"
                << Theme::reset() << Theme::fg(Theme::NEON_PINK) << " │" << Theme::reset() << "\n";
            out << "     " << Theme::fg(Theme::NEON_PINK) << "╰"
                << std::string(btnWidth + 2, '-') << "╯" << Theme::reset() << "\n";
        } else {
            out << "     " << Theme::fg(Theme::FG_MUTED) << "╭"
                << std::string(btnWidth + 2, '-') << "╮" << Theme::reset() << "\n";
            out << "     " << Theme::fg(Theme::FG_MUTED) << "│ "
                << Theme::fg(Theme::FG_BRIGHT) << "  "
                << padCenter(label, btnWidth - 4) << "  "
                << Theme::reset() << Theme::fg(Theme::FG_MUTED) << " │" << Theme::reset() << "\n";
            out << "     " << Theme::fg(Theme::FG_MUTED) << "╰"
                << std::string(btnWidth + 2, '-') << "╯" << Theme::reset() << "\n";
        }
    };

    renderButton("N E W   G A M E", selected == 'n');
    renderButton("L E A D E R B O A R D", selected == 'l');
    renderButton("E X I T", selected == 'e');

    out << "\n";
    if (showHelp) {
        out << "   " << Theme::fg(Theme::NEON_AMBER)
            << "▶ Use [W/S] or Arrow keys to select, [ENTER] to confirm"
            << Theme::reset() << "    \n";
    } else {
        out << "   " << Theme::fg(Theme::FG_MUTED)
            << "  Navigate: [W/S] or [↑/↓]  •  Confirm: [ENTER]"
            << Theme::reset() << "        \n";
    }
    out << "\n";

    std::cout << out.str() << std::flush;
}

void renderBoard(const Board& board, int bestScore, const std::string& remainingTime) {
    std::ostringstream out;
    out << Theme::homeCursor();

    int n = board.getSize();
    int currentScore = board.getScore();
    int biggestTile = board.getBiggestTile();

    int cellWidth = std::max(7, digitCount(biggestTile) + 2);

    // 1. Sleek Top Dashboard: SCORE | BEST | TIME
    std::string sScore = formatNumber(currentScore);
    std::string sBest = formatNumber(bestScore);
    std::string sTime = remainingTime.empty() ? "ENDLESS" : remainingTime;

    out << "\n";
    out << "  " << Theme::fg(Theme::NEON_CYAN) << Theme::bold() << "✦ 2048 ARCADE" << Theme::reset()
        << "  " << Theme::fg(Theme::FG_MUTED) << "•" << Theme::reset()
        << "  " << Theme::fg(Theme::NEON_GREEN) << "MAX: " << biggestTile << Theme::reset() << "\n\n";

    int cardW = 14;
    auto cardTop = [&](int w) {
        return Theme::fg(Theme::FG_MUTED) + "╭" + std::string(w, '-') + "╮" + Theme::reset();
    };
    auto cardBot = [&](int w) {
        return Theme::fg(Theme::FG_MUTED) + "╰" + std::string(w, '-') + "╯" + Theme::reset();
    };

    out << "  " << cardTop(cardW) << "  " << cardTop(cardW) << "  " << cardTop(cardW) << "\n";

    out << "  " << Theme::fg(Theme::FG_MUTED) << "│"
        << Theme::fg(Theme::NEON_AMBER) << Theme::bold() << padCenter("SCORE", cardW)
        << Theme::reset() << Theme::fg(Theme::FG_MUTED) << "│"
        << "  " << Theme::fg(Theme::FG_MUTED) << "│"
        << Theme::fg(Theme::NEON_PINK) << Theme::bold() << padCenter("BEST", cardW)
        << Theme::reset() << Theme::fg(Theme::FG_MUTED) << "│"
        << "  " << Theme::fg(Theme::FG_MUTED) << "│"
        << Theme::fg(Theme::NEON_CYAN) << Theme::bold() << padCenter("TIME", cardW)
        << Theme::reset() << Theme::fg(Theme::FG_MUTED) << "│\n";

    out << "  " << Theme::fg(Theme::FG_MUTED) << "│"
        << Theme::fg(Theme::FG_BRIGHT) << Theme::bold() << padCenter(sScore, cardW)
        << Theme::reset() << Theme::fg(Theme::FG_MUTED) << "│"
        << "  " << Theme::fg(Theme::FG_MUTED) << "│"
        << Theme::fg(Theme::FG_BRIGHT) << Theme::bold() << padCenter(sBest, cardW)
        << Theme::reset() << Theme::fg(Theme::FG_MUTED) << "│"
        << "  " << Theme::fg(Theme::FG_MUTED) << "│"
        << Theme::fg(Theme::FG_BRIGHT) << Theme::bold() << padCenter(sTime, cardW)
        << Theme::reset() << Theme::fg(Theme::FG_MUTED) << "│\n";

    out << "  " << cardBot(cardW) << "  " << cardBot(cardW) << "  " << cardBot(cardW) << "\n\n";

    // 2. The Board Grid
    auto borderCol = Theme::fg({70, 80, 115});

    // Top border
    out << "  " << borderCol << "╭";
    for (int j = 0; j < n; ++j) {
        for (int k = 0; k < cellWidth; ++k) out << "─";
        if (j < n - 1) out << "┬";
    }
    out << "╮" << Theme::reset() << "\n";

    bool tallCells = (n <= 5);

    for (int i = 0; i < n; ++i) {
        // Upper cell padding row
        if (tallCells) {
            out << "  " << borderCol << "│" << Theme::reset();
            for (int j = 0; j < n; ++j) {
                int val = board.getCell(i, j);
                auto style = Theme::getTileStyle(val);
                out << Theme::bg(style.bg) << std::string(cellWidth, ' ') << Theme::reset();
                out << borderCol << "│" << Theme::reset();
            }
            out << "\n";
        }

        // Center content row
        out << "  " << borderCol << "│" << Theme::reset();
        for (int j = 0; j < n; ++j) {
            int val = board.getCell(i, j);
            auto style = Theme::getTileStyle(val);

            std::string content;
            if (val == 0) {
                content = padCenter("·", cellWidth);
            } else {
                content = padCenter(std::to_string(val), cellWidth);
            }

            out << Theme::bg(style.bg)
                << Theme::fg(style.fg);
            if (style.bold) out << Theme::bold();
            out << content << Theme::reset();
            out << borderCol << "│" << Theme::reset();
        }
        out << "\n";

        // Lower cell padding row
        if (tallCells) {
            out << "  " << borderCol << "│" << Theme::reset();
            for (int j = 0; j < n; ++j) {
                int val = board.getCell(i, j);
                auto style = Theme::getTileStyle(val);
                out << Theme::bg(style.bg) << std::string(cellWidth, ' ') << Theme::reset();
                out << borderCol << "│" << Theme::reset();
            }
            out << "\n";
        }

        // Row separator
        if (i < n - 1) {
            out << "  " << borderCol << "├";
            for (int j = 0; j < n; ++j) {
                for (int k = 0; k < cellWidth; ++k) out << "─";
                if (j < n - 1) out << "┼";
            }
            out << "┤" << Theme::reset() << "\n";
        }
    }

    // Bottom border
    out << "  " << borderCol << "╰";
    for (int j = 0; j < n; ++j) {
        for (int k = 0; k < cellWidth; ++k) out << "─";
        if (j < n - 1) out << "┴";
    }
    out << "╯" << Theme::reset() << "\n\n";

    // Footer bar
    out << "  " << Theme::fg(Theme::FG_MUTED)
        << "Controls: " << Theme::fg(Theme::NEON_CYAN) << "[W/A/S/D / Arrows]"
        << Theme::fg(Theme::FG_MUTED) << " Slide  •  "
        << Theme::fg(Theme::NEON_AMBER) << "[M]"
        << Theme::fg(Theme::FG_MUTED) << " Music  •  "
        << Theme::fg(Theme::NEON_AMBER) << "[B]"
        << Theme::fg(Theme::FG_MUTED) << " Menu"
        << Theme::reset() << "         \n";

    std::cout << out.str() << std::flush;
}

void renderHowToPlay() {
    resetScreen();
    std::ostringstream out;

    out << "\n";
    out << "  " << Theme::fg(Theme::NEON_CYAN) << "╭──────────────────────── HOW TO PLAY ────────────────────────╮" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_CYAN) << "│" << Theme::reset()
        << Theme::fg(Theme::NEON_AMBER) << Theme::bold() << padCenter("2048 RETRO ARCADE GUIDE", 60)
        << Theme::reset() << Theme::fg(Theme::NEON_CYAN) << "│" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_CYAN) << "├────────────────────────────────────────────────────────────┤" << Theme::reset() << "\n";

    auto printGuideLine = [&](const std::string& text) {
        out << "  " << Theme::fg(Theme::NEON_CYAN) << "│ " << Theme::reset()
            << Theme::fg(Theme::FG_BRIGHT) << std::left << std::setw(58) << text
            << Theme::reset() << Theme::fg(Theme::NEON_CYAN) << " │" << Theme::reset() << "\n";
    };

    printGuideLine("• Use [W/A/S/D] or Arrow Keys to slide all tiles.");
    printGuideLine("• When two tiles with the same number touch, they");
    printGuideLine("  merge into one tile with doubled value!");
    printGuideLine("• Every move spawns a new 2 or 4 tile in an empty spot.");
    printGuideLine("• Merge your way up to create the legendary 2048 tile!");
    printGuideLine("• Press [B] at any time during play to return to menu.");
    printGuideLine("");
    printGuideLine("Are you ready to claim the high score?");

    out << "  " << Theme::fg(Theme::NEON_CYAN) << "╰────────────────────────────────────────────────────────────╯" << Theme::reset() << "\n\n";
    out << "  " << Theme::fg(Theme::NEON_PINK) << Theme::bold() << "▶ Press any key to continue..." << Theme::reset();

    std::cout << out.str() << std::flush;
}

void renderWinnerPrompt() {
    std::ostringstream out;
    out << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "╔════════════════════════════════════════════════════════════╗" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "║" << Theme::reset()
        << Theme::fg(Theme::NEON_AMBER) << Theme::bold()
        << padCenter("★ ★ ★  V I C T O R Y !  ★ ★ ★", 60)
        << Theme::reset() << Theme::fg(Theme::NEON_AMBER) << "║" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "║" << Theme::reset()
        << Theme::fg(Theme::FG_BRIGHT)
        << padCenter("YOU CREATED THE 2048 TILE!", 60)
        << Theme::reset() << Theme::fg(Theme::NEON_AMBER) << "║" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "║" << Theme::reset()
        << Theme::fg(Theme::NEON_CYAN)
        << padCenter("Do you want to continue playing in endless mode? (y/n)", 60)
        << Theme::reset() << Theme::fg(Theme::NEON_AMBER) << "║" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "╚════════════════════════════════════════════════════════════╝" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << " >> " << Theme::reset();

    std::cout << out.str() << std::flush;
}

void renderGameOver() {
    std::ostringstream out;
    out << "\n";
    out << "  " << Theme::fg(Theme::NEON_RED) << "╔════════════════════════════════════════════════════════════╗" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_RED) << "║" << Theme::reset()
        << Theme::fg(Theme::NEON_RED) << Theme::bold()
        << padCenter("☠   G A M E   O V E R   ☠", 60)
        << Theme::reset() << Theme::fg(Theme::NEON_RED) << "║" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_RED) << "║" << Theme::reset()
        << Theme::fg(Theme::FG_BRIGHT)
        << padCenter("No more moves available on the board!", 60)
        << Theme::reset() << Theme::fg(Theme::NEON_RED) << "║" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_RED) << "║" << Theme::reset()
        << Theme::fg(Theme::FG_MUTED)
        << padCenter("Press any key to view results...", 60)
        << Theme::reset() << Theme::fg(Theme::NEON_RED) << "║" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_RED) << "╚════════════════════════════════════════════════════════════╝" << Theme::reset() << "\n";

    std::cout << out.str() << std::flush;
}

int renderSizeSelection(const std::vector<int>& playedSizes) {
    resetScreen();
    std::ostringstream out;

    out << "\n";
    out << "  " << Theme::fg(Theme::NEON_CYAN) << "╭──────────────────── SELECT BOARD SIZE ────────────────────╮" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_CYAN) << "│" << Theme::reset()
        << Theme::fg(Theme::NEON_AMBER) << Theme::bold() << padCenter("Choose Grid Size for Leaderboard", 58)
        << Theme::reset() << Theme::fg(Theme::NEON_CYAN) << "│" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_CYAN) << "╰───────────────────────────────────────────────────────────╯" << Theme::reset() << "\n\n";

    for (int size : playedSizes) {
        out << "      " << Theme::fg(Theme::NEON_PINK) << "▶ "
            << Theme::fg(Theme::FG_BRIGHT) << Theme::bold() << size << " x " << size
            << Theme::reset() << "\n";
    }
    out << "\n  " << Theme::fg(Theme::NEON_CYAN) << "Enter size (e.g. 4): " << Theme::reset();
    std::cout << out.str() << std::flush;

    std::string ch;
    std::cin >> ch;
    try {
        return std::stoi(ch);
    } catch (...) {
        return 4;
    }
}

void renderLeaderboard(const std::vector<PlayerRecord>& records, int boardSize) {
    resetScreen();
    std::ostringstream out;

    int tableWidth = 66;
    out << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "╭" << std::string(tableWidth, '-') << "╮" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "│" << Theme::reset()
        << Theme::fg(Theme::NEON_AMBER) << Theme::bold()
        << padCenter("★ ★ ★   H A L L   O F   F A M E   ★ ★ ★", tableWidth)
        << Theme::reset() << Theme::fg(Theme::NEON_AMBER) << "│" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "│" << Theme::reset()
        << Theme::fg(Theme::NEON_CYAN)
        << padCenter("Board Size: " + std::to_string(boardSize) + " x " + std::to_string(boardSize), tableWidth)
        << Theme::reset() << Theme::fg(Theme::NEON_AMBER) << "│" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "├──────┬───────────────────────────────┬──────────────┬──────┤" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "│ "
        << Theme::fg(Theme::FG_BRIGHT) << Theme::bold() << "RANK" << Theme::reset()
        << Theme::fg(Theme::NEON_AMBER) << " │ "
        << Theme::fg(Theme::FG_BRIGHT) << Theme::bold() << std::left << std::setw(29) << "PLAYER NAME" << Theme::reset()
        << Theme::fg(Theme::NEON_AMBER) << " │ "
        << Theme::fg(Theme::FG_BRIGHT) << Theme::bold() << std::right << std::setw(12) << "SCORE" << Theme::reset()
        << Theme::fg(Theme::NEON_AMBER) << " │ "
        << Theme::fg(Theme::FG_BRIGHT) << Theme::bold() << "GRID" << Theme::reset()
        << Theme::fg(Theme::NEON_AMBER) << " │" << Theme::reset() << "\n";
    out << "  " << Theme::fg(Theme::NEON_AMBER) << "├──────┼───────────────────────────────┼──────────────┼──────┤" << Theme::reset() << "\n";

    if (records.empty()) {
        out << "  " << Theme::fg(Theme::NEON_AMBER) << "│" << Theme::reset()
            << Theme::fg(Theme::FG_MUTED)
            << padCenter("No records found for this board size yet!", tableWidth)
            << Theme::reset() << Theme::fg(Theme::NEON_AMBER) << "│" << Theme::reset() << "\n";
    } else {
        int rank = 1;
        for (const auto& rec : records) {
            std::string rankStr = "#" + std::to_string(rank);
            std::string nameDisplay = rec.name;
            if (nameDisplay.length() > 27) nameDisplay = nameDisplay.substr(0, 24) + "...";

            Theme::Color rankColor = (rank == 1) ? Theme::NEON_AMBER :
                                     (rank == 2) ? Theme::Color{200, 210, 225} :
                                     (rank == 3) ? Theme::Color{205, 127, 50} :
                                                   Theme::FG_MUTED;

            out << "  " << Theme::fg(Theme::NEON_AMBER) << "│ "
                << Theme::fg(rankColor) << Theme::bold() << std::left << std::setw(4) << rankStr << Theme::reset()
                << Theme::fg(Theme::NEON_AMBER) << " │ "
                << Theme::fg(Theme::FG_BRIGHT) << std::left << std::setw(29) << nameDisplay << Theme::reset()
                << Theme::fg(Theme::NEON_AMBER) << " │ "
                << Theme::fg(Theme::NEON_GREEN) << Theme::bold() << std::right << std::setw(12) << formatNumber(rec.score) << Theme::reset()
                << Theme::fg(Theme::NEON_AMBER) << " │ "
                << Theme::fg(Theme::FG_MUTED) << boardSize << "x" << boardSize << Theme::reset()
                << Theme::fg(Theme::NEON_AMBER) << "  │" << Theme::reset() << "\n";

            rank++;
            if (rank > 10) break;
        }
    }

    out << "  " << Theme::fg(Theme::NEON_AMBER) << "╰──────┴───────────────────────────────┴──────────────┴──────╯" << Theme::reset() << "\n\n";
    out << "  " << Theme::fg(Theme::NEON_PINK) << Theme::bold() << "▶ Press any key to return to menu..." << Theme::reset();

    std::cout << out.str() << std::flush;
}

} // namespace UI
