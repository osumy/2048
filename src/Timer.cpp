#include "Timer.hpp"
#include <sstream>

GameTimer::~GameTimer() {
    stop();
}

void GameTimer::worker() {
    while (m_running.load()) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if (!m_running.load()) {
            break;
        }
        int currentSec = m_remainingSeconds.load();
        if (currentSec > 0) {
            m_remainingSeconds.store(currentSec - 1);
        } else {
            m_timeUp.store(true);
            m_running.store(false);
            break;
        }
    }
}

void GameTimer::start(int minutes, int seconds) {
    stop();
    m_remainingSeconds.store(minutes * 60 + seconds);
    m_timeUp.store(false);
    m_running.store(true);
    m_thread = std::thread(&GameTimer::worker, this);
}

void GameTimer::stop() {
    if (m_running.load()) {
        m_running.store(false);
    }
    if (m_thread.joinable()) {
        m_thread.join();
    }
}

bool GameTimer::isTimeUp() const {
    return m_timeUp.load();
}

std::string GameTimer::getRemainingFormatted() const {
    int total = m_remainingSeconds.load();
    if (total < 0) total = 0;
    int m = total / 60;
    int s = total % 60;
    std::ostringstream oss;
    oss << m << ":" << (s < 10 ? "0" : "") << s;
    return oss.str();
}
