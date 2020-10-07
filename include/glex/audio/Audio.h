#pragma once

#ifdef DREAMCAST
#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>
#endif

#include <string>

/*
 * Current limitations:
 *   - Only uncompressed 44.1Khz WAV is currently supported (more formats coming later)
 */

class Audio {
public:
    static void audioInit();

    Audio(std::string path) { _path = path; }
    ~Audio();

    bool load();
    void unload();

    void play();
    void pause();
    void stop();

private:
    static bool _isAudioInitialized;
    std::string _path = "";
    bool _isLoaded = false;
    // bool _isPlaying = false;

#ifdef DREAMCAST
    sfxhnd_t _handle = 0;
#endif
};
