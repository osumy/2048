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

void startBGM();
void stopBGM();
void toggleBGM();
bool isBGMPlaying();

void playSFX(SFX sound);
void toggleSFX();
bool isSFXEnabled();

} // namespace Audio
