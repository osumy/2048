#include "Audio.hpp"
#include <vector>
#include <cmath>
#include <cstdint>

#ifdef _WIN32
#include <windows.h>
#include <mmsystem.h>
#ifdef _MSC_VER
#pragma comment(lib, "winmm.lib")
#endif
#endif

namespace Audio {

static bool s_initialized = false;
static bool s_sfxEnabled = true;

#ifdef _WIN32
static std::vector<uint8_t> s_moveWav;
static std::vector<uint8_t> s_mergeWav;
static std::vector<uint8_t> s_winWav;
static std::vector<uint8_t> s_overWav;

static std::vector<uint8_t> createWav(int sampleRate, int durationMs, int freq1, int freq2 = 0) {
    int totalSamples = (sampleRate * durationMs) / 1000;
    std::vector<uint8_t> pcm;
    pcm.reserve(totalSamples);

    for (int i = 0; i < totalSamples; ++i) {
        double t = static_cast<double>(i) / sampleRate;
        double env = 1.0 - (static_cast<double>(i) / totalSamples);

        int curFreq = (freq2 > 0 && i > totalSamples / 2) ? freq2 : freq1;
        double wave = (std::sin(2.0 * 3.141592653589793 * curFreq * t) > 0.0) ? 1.0 : -1.0;

        int sample = static_cast<int>(128.0 + 80.0 * wave * env);
        if (sample < 0) sample = 0;
        if (sample > 255) sample = 255;
        pcm.push_back(static_cast<uint8_t>(sample));
    }

    std::vector<uint8_t> wav;
    wav.reserve(44 + pcm.size());

    // "RIFF"
    wav.push_back('R'); wav.push_back('I'); wav.push_back('F'); wav.push_back('F');
    uint32_t chunkSize = static_cast<uint32_t>(36 + pcm.size());
    wav.push_back(chunkSize & 0xFF);
    wav.push_back((chunkSize >> 8) & 0xFF);
    wav.push_back((chunkSize >> 16) & 0xFF);
    wav.push_back((chunkSize >> 24) & 0xFF);

    // "WAVE"
    wav.push_back('W'); wav.push_back('A'); wav.push_back('V'); wav.push_back('E');

    // "fmt "
    wav.push_back('f'); wav.push_back('m'); wav.push_back('t'); wav.push_back(' ');
    uint32_t sub1Size = 16;
    wav.push_back(sub1Size & 0xFF); wav.push_back(0); wav.push_back(0); wav.push_back(0);
    wav.push_back(1); wav.push_back(0); // PCM
    wav.push_back(1); wav.push_back(0); // Mono
    uint32_t sr = sampleRate;
    wav.push_back(sr & 0xFF);
    wav.push_back((sr >> 8) & 0xFF);
    wav.push_back((sr >> 16) & 0xFF);
    wav.push_back((sr >> 24) & 0xFF);
    wav.push_back(sr & 0xFF);
    wav.push_back((sr >> 8) & 0xFF);
    wav.push_back((sr >> 16) & 0xFF);
    wav.push_back((sr >> 24) & 0xFF);
    wav.push_back(1); wav.push_back(0); // BlockAlign
    wav.push_back(8); wav.push_back(0); // BitsPerSample

    // "data"
    wav.push_back('d'); wav.push_back('a'); wav.push_back('t'); wav.push_back('a');
    uint32_t dataSize = static_cast<uint32_t>(pcm.size());
    wav.push_back(dataSize & 0xFF);
    wav.push_back((dataSize >> 8) & 0xFF);
    wav.push_back((dataSize >> 16) & 0xFF);
    wav.push_back((dataSize >> 24) & 0xFF);

    wav.insert(wav.end(), pcm.begin(), pcm.end());
    return wav;
}
#endif

void init() {
    if (s_initialized) return;

#ifdef _WIN32
    // Silence any previous audio
    PlaySoundA(NULL, NULL, 0);

    // Self-terminating sound buffers:
    // Move: crisp 25ms 850Hz key click
    s_moveWav = createWav(8000, 25, 850);
    // Merge: cheerful 55ms two-tone chime (900Hz -> 1350Hz)
    s_mergeWav = createWav(8000, 55, 900, 1350);
    // Win: bright 110ms fanfare
    s_winWav = createWav(8000, 110, 1200, 1600);
    // GameOver: descending 90ms tone
    s_overWav = createWav(8000, 90, 450, 280);
#endif

    s_initialized = true;
}

void shutdown() {
    if (!s_initialized) return;

#ifdef _WIN32
    PlaySoundA(NULL, NULL, 0);
#endif

    s_initialized = false;
}

void toggleSFX() {
    s_sfxEnabled = !s_sfxEnabled;
    if (!s_sfxEnabled) {
#ifdef _WIN32
        PlaySoundA(NULL, NULL, 0);
#endif
    }
}

bool isSFXEnabled() {
    return s_sfxEnabled;
}

void playSFX(SFX sound) {
    if (!s_sfxEnabled) return;

#ifdef _WIN32
    if (!s_initialized) init();

    const std::vector<uint8_t>* target = nullptr;
    switch (sound) {
        case SFX::Move:     target = &s_moveWav;  break;
        case SFX::Merge:    target = &s_mergeWav; break;
        case SFX::Win:      target = &s_winWav;   break;
        case SFX::GameOver: target = &s_overWav;  break;
    }

    if (target && !target->empty()) {
        // SND_MEMORY | SND_ASYNC plays once and terminates automatically
        PlaySoundA(reinterpret_cast<LPCSTR>(target->data()), NULL, SND_MEMORY | SND_ASYNC);
    }
#endif
}

} // namespace Audio
