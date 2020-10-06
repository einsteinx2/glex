#ifdef DREAMCAST

#include "glex/audio/Audio.h"
#include "glex/common/path.h"
#include "glex/common/log.h"

#include <kos/mutex.h>

/*
 * TODO: 
 *   - Expose private state variables with getters
 *   - Play sounds in a thread or thread(s)
 *   - Add volume and pan controls
 *   - Add option to loop (limited and unlimited)
 *   - Add more option for stuff like channels, etc
 *   - If needed, create options for sound fx vs background music (maybe diff classes? or just diff threads?)
 */

// Not needed as sound effects can just play on the main thread without blocking
// void* _audioThreadLoop(void* params);

// class AudioThread {
// public:
//     AudioThread() {
//         int success = mutex_init(_mutex, MUTEX_TYPE_RECURSIVE);
//         if (success != 0) {
//             ERROR_PRINTLN("AudioThread - Failed to create mutex: %d", success);
//             return;
//         }

//         _thread = thd_create(0, _audioThreadLoop, this);
//     }

//     ~AudioThread() {
//          // Stop the thread
//         lockMutex();
//         _shouldExitThread = true;
//         unlockMutex();
//         thd_join(_thread, NULL);
//         // TODO: Neeed to call thd_destroy since this is a joinable thread?
//         thd_destroy(_thread);
        
//         // Clean up the mutex
//         mutex_destroy(_mutex);
//     }

//     int lockMutex() { 
//         return mutex_lock(_mutex); 
//     }

//     int unlockMutex() {
//         return mutex_unlock(_mutex);
//     }

//     bool shouldExitThread() {
//         lockMutex();
//         bool shouldExit = _shouldExitThread;
//         unlockMutex();
//         return shouldExit;
//     }

// private:
//     kthread_t* _thread = NULL;
//     mutex_t* _mutex = NULL;
//     bool _shouldExitThread = false;
// };

// void* _audioThreadLoop(void* params) {
//     AudioThread* audioThread = (AudioThread*)params;
//     while (!audioThread->shouldExitThread()) {

//     }
// }

// Static audio initialize function, only needed to be run once for all audio objects
bool Audio::_isAudioInitialized = false;
void Audio::audioInit() {
    if (_isAudioInitialized) return;

    snd_init();
    _isAudioInitialized = true;
}

Audio::~Audio() {
    stop();
    if (_isLoaded) {
        unload();
    }
}

bool Audio::load() {
    if (!_isAudioInitialized) {
        audioInit();
    }

    _handle = snd_sfx_load(glex::targetPlatformPath(_path).c_str());
    DEBUG_PRINTLN("Audio::load - _handle: %u", _handle);
    _isLoaded = _handle > 0;
    return _isLoaded;
}

void Audio::unload() {
    if (!_isLoaded || _handle <= 0) return;

    snd_sfx_unload(_handle);
    _handle = 0;
    _isLoaded = false;
}

void Audio::play() {
    if (!_isLoaded || _handle <= 0) return;

    snd_sfx_play(_handle, 255, 128);
}

void Audio::pause() {
    // TODO: See if it's possible to pause samples
    stop();
}

void Audio::stop() {
    if (!_isLoaded || _handle <= 0) return;

    snd_sfx_stop(_handle);
}

#endif
