#pragma once

#include <atomic>
#include <chrono>
#include <string>
#include <thread>

class GameTimer {
public:
    GameTimer() = default;
    ~GameTimer();

    // Disallow copying
    GameTimer(const GameTimer&) = delete;
    GameTimer& operator=(const GameTimer&) = delete;

    void start(int minutes, int seconds = 0);
    void stop();
    bool isTimeUp() const;
    std::string getRemainingFormatted() const;

private:
    void worker();

    std::atomic<bool> m_running{false};
    std::atomic<bool> m_timeUp{false};
    std::atomic<int> m_remainingSeconds{0};
    std::thread m_thread;
};
