#include "Board.hpp"
#include <cstdlib>

Board::Board(int size) {
    reset(size);
}

void Board::reset(int size) {
    m_size = (size < 2) ? 2 : size;
    m_score = 0;
    m_grid.assign(m_size, std::vector<int>(m_size, 0));
    spawnTile();
    spawnTile();
}

int Board::getEmptyCellCount() const {
    int count = 0;
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            if (m_grid[i][j] == 0) {
                ++count;
            }
        }
    }
    return count;
}

bool Board::isFull() const {
    return getEmptyCellCount() == 0;
}

bool Board::spawnTile() {
    std::vector<std::pair<int, int>> emptyCells;
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            if (m_grid[i][j] == 0) {
                emptyCells.emplace_back(i, j);
            }
        }
    }

    if (emptyCells.empty()) {
        return false;
    }

    int randIdx = std::rand() % emptyCells.size();
    int val = (std::rand() % 4 == 3) ? 4 : 2;
    m_grid[emptyCells[randIdx].first][emptyCells[randIdx].second] = val;
    return true;
}

int Board::getBiggestTile() const {
    int maxVal = m_grid[0][0];
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size; ++j) {
            if (m_grid[i][j] > maxVal) {
                maxVal = m_grid[i][j];
            }
        }
    }
    return maxVal;
}

bool Board::canMove() const {
    // If there's an empty cell, moves are possible
    if (!isFull()) {
        return true;
    }

    // Check horizontal neighbors
    for (int i = 0; i < m_size; ++i) {
        for (int j = 0; j < m_size - 1; ++j) {
            if (m_grid[i][j] == m_grid[i][j + 1]) {
                return true;
            }
        }
    }

    // Check vertical neighbors
    for (int j = 0; j < m_size; ++j) {
        for (int i = 0; i < m_size - 1; ++i) {
            if (m_grid[i][j] == m_grid[i + 1][j]) {
                return true;
            }
        }
    }

    return false;
}

void Board::moveUp() {
    for (int j = 0; j < m_size; ++j) {
        for (int x = 0; x < m_size - 1; ++x) {
            for (int i = 0; i < m_size - 1; ++i) {
                if (m_grid[i][j] == 0 && m_grid[i + 1][j] != 0) {
                    m_grid[i][j] = m_grid[i + 1][j];
                    m_grid[i + 1][j] = 0;
                }
            }
        }

        for (int i = 0; i < m_size - 1; ++i) {
            if (m_grid[i][j] != 0 && m_grid[i][j] == m_grid[i + 1][j]) {
                m_grid[i][j] *= 2;
                m_score += m_grid[i][j];
                for (int I = 1; i + I <= m_size - 1; ++I) {
                    if (i + I == m_size - 1) {
                        m_grid[i + I][j] = 0;
                    } else {
                        m_grid[i + I][j] = m_grid[i + I + 1][j];
                    }
                }
            }
        }
    }
}

void Board::moveDown() {
    for (int j = 0; j < m_size; ++j) {
        for (int x = 0; x < m_size - 1; ++x) {
            for (int i = m_size - 1; i > 0; --i) {
                if (m_grid[i][j] == 0 && m_grid[i - 1][j] != 0) {
                    m_grid[i][j] = m_grid[i - 1][j];
                    m_grid[i - 1][j] = 0;
                }
            }
        }

        for (int i = m_size - 1; i > 0; --i) {
            if (m_grid[i][j] != 0 && m_grid[i][j] == m_grid[i - 1][j]) {
                m_grid[i][j] *= 2;
                m_score += m_grid[i][j];
                for (int I = 1; i - I >= 0; ++I) {
                    if (i - I == 0) {
                        m_grid[i - I][j] = 0;
                    } else {
                        m_grid[i - I][j] = m_grid[i - I - 1][j];
                    }
                }
            }
        }
    }
}

void Board::moveRight() {
    for (int i = 0; i < m_size; ++i) {
        for (int x = 0; x < m_size - 1; ++x) {
            for (int j = m_size - 1; j > 0; --j) {
                if (m_grid[i][j] == 0 && m_grid[i][j - 1] != 0) {
                    m_grid[i][j] = m_grid[i][j - 1];
                    m_grid[i][j - 1] = 0;
                }
            }
        }

        for (int j = m_size - 1; j > 0; --j) {
            if (m_grid[i][j] != 0 && m_grid[i][j] == m_grid[i][j - 1]) {
                m_grid[i][j] *= 2;
                m_score += m_grid[i][j];
                for (int J = 1; j - J >= 0; ++J) {
                    if (j - J == 0) {
                        m_grid[i][j - J] = 0;
                    } else {
                        m_grid[i][j - J] = m_grid[i][j - J - 1];
                    }
                }
            }
        }
    }
}

void Board::moveLeft() {
    for (int i = 0; i < m_size; ++i) {
        for (int x = 0; x < m_size - 1; ++x) {
            for (int j = 0; j < m_size - 1; ++j) {
                if (m_grid[i][j] == 0 && m_grid[i][j + 1] != 0) {
                    m_grid[i][j] = m_grid[i][j + 1];
                    m_grid[i][j + 1] = 0;
                }
            }
        }

        for (int j = 0; j < m_size - 1; ++j) {
            if (m_grid[i][j] != 0 && m_grid[i][j] == m_grid[i][j + 1]) {
                m_grid[i][j] *= 2;
                m_score += m_grid[i][j];
                for (int J = 1; j + J <= m_size - 1; ++J) {
                    if (j + J == m_size - 1) {
                        m_grid[i][j + J] = 0;
                    } else {
                        m_grid[i][j + J] = m_grid[i][j + J + 1];
                    }
                }
            }
        }
    }
}

bool Board::move(Direction dir) {
    auto previous = m_grid;

    switch (dir) {
        case Direction::Up:    moveUp(); break;
        case Direction::Down:  moveDown(); break;
        case Direction::Left:  moveLeft(); break;
        case Direction::Right: moveRight(); break;
    }

    bool changed = (m_grid != previous);
    if (changed && getEmptyCellCount() > 0) {
        spawnTile();
    }
    return changed;
}
