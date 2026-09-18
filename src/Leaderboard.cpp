#include "Leaderboard.hpp"
#include <fstream>
#include <algorithm>
#include <set>

Leaderboard::Leaderboard(std::string filePath)
    : m_filePath(std::move(filePath)) {
    load();
}

void Leaderboard::load() {
    m_records.clear();
    std::ifstream file(m_filePath);
    if (!file.is_open()) {
        return;
    }

    std::string name;
    std::string scoreStr;
    std::string sizeStr;

    while (std::getline(file, name)) {
        if (!std::getline(file, scoreStr) || !std::getline(file, sizeStr)) {
            break;
        }

        // Handle possible carriage returns or empty entries
        if (name.empty() && scoreStr.empty() && sizeStr.empty()) {
            continue;
        }

        try {
            int score = std::stoi(scoreStr);
            int size = std::stoi(sizeStr);
            m_records.push_back({name, score, size});
        } catch (...) {
            // Ignore malformed record
        }
    }
}

void Leaderboard::addRecord(const std::string& name, int score, int boardSize) {
    m_records.push_back({name, score, boardSize});

    // Append to file directly
    std::ofstream file(m_filePath, std::ios::app);
    if (file.is_open()) {
        file << name << "\n" << score << "\n" << boardSize << "\n";
    }
}

int Leaderboard::getBestScore(int boardSize) const {
    int best = 0;
    for (const auto& rec : m_records) {
        if (rec.boardSize == boardSize && rec.score > best) {
            best = rec.score;
        }
    }
    return best;
}

std::vector<PlayerRecord> Leaderboard::getRecordsForSize(int boardSize) const {
    std::vector<PlayerRecord> filtered;
    for (const auto& rec : m_records) {
        if (rec.boardSize == boardSize) {
            filtered.push_back(rec);
        }
    }

    // Sort descending by score
    std::sort(filtered.begin(), filtered.end(), [](const PlayerRecord& a, const PlayerRecord& b) {
        return a.score > b.score;
    });

    return filtered;
}

std::vector<int> Leaderboard::getPlayedSizes() const {
    std::set<int> uniqueSizes;
    for (const auto& rec : m_records) {
        uniqueSizes.insert(rec.boardSize);
    }
    return {uniqueSizes.begin(), uniqueSizes.end()};
}
