#ifdef DREAMCAST

#include "glex/audio/AudioSoundEffect.h"
#include "glex/common/path.h"
#include "glex/common/log.h"

#include <dc/sound/sound.h>
#include <dc/sound/sfxmgr.h>

// Static audio initialize function, only needed to be run once for all audio objects
namespace {
    static bool _isInitialized = false;
    void _initialize() {
        if (_isInitialized) return;

        snd_init();
        // int channel = snd_sfx_chn_alloc();
        // DEBUG_PRINTLN("snd_sfx_chn_alloc: %d", channel);
        _isInitialized = true;
    }
}

AudioSoundEffect::~AudioSoundEffect() {
    stop();
    unload();
}

bool AudioSoundEffect::load() {
    if (_isLoaded || _handle != SFXHND_INVALID) return true;

    // Initialize the audio system if not yet initialized
    _initialize();

    DEBUG_PRINTLN("Loading audio sound effect from path: %s", glex::targetPlatformPath(_path).c_str());
    _handle = snd_sfx_load(glex::targetPlatformPath(_path).c_str());
    DEBUG_PRINTLN("AudioSoundEffect::load - _handle: %lu", _handle);
    _isLoaded = _handle != SFXHND_INVALID;
    return _isLoaded;
}

void AudioSoundEffect::unload() {
    if (!_isLoaded || _handle == SFXHND_INVALID) return;

    snd_sfx_unload(_handle);
    _handle = 0;
    _isLoaded = false;
}

void AudioSoundEffect::play() {
    // DEBUG_PRINTLN("AudioSoundEffect::play - _isLoaded: %d  _handle: %d", _isLoaded, _handle);
    if (!_isLoaded || _handle == SFXHND_INVALID) return;

    // DEBUG_PRINTLN("AudioSoundEffect::play - calling snd_sfx_play");
    snd_sfx_play(_handle, 255, 128);
    // int channel = snd_sfx_chn_alloc();
    // DEBUG_PRINTLN("play snd_sfx_chn_alloc: %d", channel);
    // snd_sfx_play_chn(channel, _handle, 255, 128);
}

void AudioSoundEffect::pause() {
    // TODO: See if it's possible to pause samples
    stop();
}

void AudioSoundEffect::stop() {
    if (!_isLoaded || _handle == SFXHND_INVALID) return;

    snd_sfx_stop(_handle);
}

#endif
