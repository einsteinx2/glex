#pragma once

#include "glex/audio/Audio.h"

#include <dc/sound/sfxmgr.h>

#include <string>

/*
 * Current limitations:
 *   - Only uncompressed 44.1Khz WAV is currently supported (more formats coming later)
 */

class AudioSoundEffect : Audio {
public:
    AudioSoundEffect(std::string path) : Audio(path) {};
    ~AudioSoundEffect();

    virtual bool load();
    virtual void unload();

    virtual void play();
    virtual void pause();
    virtual void stop();

private:
#ifdef DREAMCAST
    sfxhnd_t _handle = SFXHND_INVALID;
#endif
};
