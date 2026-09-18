#pragma once

#include <string>
#include <vector>

struct PlayerRecord {
    std::string name;
    int score{0};
    int boardSize{4};
};

class Leaderboard {
public:
    explicit Leaderboard(std::string filePath = "rec.txt");

    void load();
    void addRecord(const std::string& name, int score, int boardSize);
    [[nodiscard]] int getBestScore(int boardSize) const;
    [[nodiscard]] std::vector<PlayerRecord> getRecordsForSize(int boardSize) const;
    [[nodiscard]] std::vector<int> getPlayedSizes() const;

private:
    std::string m_filePath;
    std::vector<PlayerRecord> m_records;
};
