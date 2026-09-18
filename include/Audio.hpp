#pragma once

namespace Audio {

enum class SFX {
    Move,
    Merge,
    Win,
    GameOver
};

void init();
void shutdown();

// Background Music (BGM) controls
void startBGM();
void stopBGM();
void toggleBGM();
bool isBGMPlaying();

// Sound Effects (SFX) controls
void playSFX(SFX sound);
void toggleSFX();
bool isSFXEnabled();

} // namespace Audio
