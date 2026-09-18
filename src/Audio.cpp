#include "Audio.hpp"

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
static HMIDIOUT s_hMidi = nullptr;
#endif

void init() {
    if (s_initialized) return;

#ifdef _WIN32
    // Open MIDI synthesizer device for zero-latency 8-bit arcade key sounds
    if (midiOutOpen(&s_hMidi, MIDI_MAPPER, 0, 0, CALLBACK_NULL) == MMSYSERR_NOERROR) {
        // Channel 0: Program 80 (Lead 1 - Square Wave / Retro 8-bit Synth)
        midiOutShortMsg(s_hMidi, 0x0050C0);
    }
#endif

    s_initialized = true;
}

void shutdown() {
    if (!s_initialized) return;

#ifdef _WIN32
    if (s_hMidi) {
        midiOutReset(s_hMidi);
        midiOutClose(s_hMidi);
        s_hMidi = nullptr;
    }
#endif

    s_initialized = false;
}

void toggleSFX() {
    s_sfxEnabled = !s_sfxEnabled;
}

bool isSFXEnabled() {
    return s_sfxEnabled;
}

void playSFX(SFX sound) {
    if (!s_sfxEnabled) return;

#ifdef _WIN32
    if (!s_initialized) init();

    if (s_hMidi) {
        switch (sound) {
            case SFX::Move:
                // Snappy arcade key click: Note A5 (81), velocity 90 on ch0
                midiOutShortMsg(s_hMidi, 0x005A5190);
                break;

            case SFX::Merge:
                // Cheerful 8-bit merge chime: Note C5 (72) then G5 (79)
                midiOutShortMsg(s_hMidi, 0x007F4890);
                midiOutShortMsg(s_hMidi, 0x007F4F90);
                break;

            case SFX::Win:
                // Victory fanfare: C5 + E5 + G5
                midiOutShortMsg(s_hMidi, 0x007F4890);
                midiOutShortMsg(s_hMidi, 0x007F4C90);
                midiOutShortMsg(s_hMidi, 0x007F4F90);
                break;

            case SFX::GameOver:
                // Low descent game over note: C3 (48)
                midiOutShortMsg(s_hMidi, 0x007F3090);
                break;
        }
    }
#endif
}

} // namespace Audio
