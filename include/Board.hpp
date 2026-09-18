#pragma once

#include <vector>
#include <utility>

enum class Direction {
    Up,
    Down,
    Left,
    Right
};

class Board {
public:
    explicit Board(int size = 4);

    void reset(int size);

    bool move(Direction dir);
    bool spawnTile();

    [[nodiscard]] bool canMove() const;
    [[nodiscard]] bool isFull() const;
    [[nodiscard]] int getBiggestTile() const;
    [[nodiscard]] int getScore() const { return m_score; }
    [[nodiscard]] int getSize() const { return m_size; }
    [[nodiscard]] int getCell(int r, int c) const { return m_grid[r][c]; }
    [[nodiscard]] const std::vector<std::vector<int>>& getGrid() const { return m_grid; }
    [[nodiscard]] int getEmptyCellCount() const;

private:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();

    int m_size{4};
    int m_score{0};
    std::vector<std::vector<int>> m_grid;
};
