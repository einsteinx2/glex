#ifdef DREAMCAST

#include "glex/audio/AudioStream.h"
#include "glex/common/path.h"
#include "glex/common/log.h"

#include <vorbis/sndoggvorbis.h>
#include <mp3/sndserver.h>

namespace {
    // MP3
    static bool _isInitializedMP3 = false;
    bool _initializeMP3() {
        if (_isInitializedMP3) return true;

        if (mp3_init() == SND_STREAM_INVALID) {
            ERROR_PRINTLN("Failed to initialize KOS MP3 (libMP3) library");
            return false;
        }
        _isInitializedMP3 = true;
        return true;
    }
    bool _shutdownMP3() {
        if (!_isInitializedMP3) return true;

        mp3_shutdown();
        snd_stream_shutdown();
        _isInitializedMP3 = false;
        return true;
    }

    // OGG
    static bool _isInitializedOGG = false;
    bool _initializeOGG() {
        if (_isInitializedOGG) return true;

        if (sndoggvorbis_init() == SND_STREAM_INVALID) {
            ERROR_PRINTLN("Failed to initialize KOS OGG Vorbis (libTremor) library");
            return false;
        }
        _isInitializedOGG = true;
        return true;
    }
    bool _shutdownOGG() {
        if (!_isInitializedOGG) return true;

        sndoggvorbis_shutdown();
        snd_stream_shutdown();
        _isInitializedOGG = false;
        return true;
    }
}

AudioStream::~AudioStream() {
    stop();
    if (_isLoaded) {
        unload();
    }
}

bool AudioStream::load() {
    if (_isLoaded) return true;

    DEBUG_PRINTLN("Loading audio stream from path: %s", glex::targetPlatformPath(_path).c_str());
    if (fileType == AudioStreamFileType::MP3) {
        // Initialize MP3 library
        _isLoaded = _initializeMP3();
    } else if (fileType == AudioStreamFileType::OGG) {
        // Initialize OGG library
        _isLoaded = _initializeOGG();

        // TODO: Queueing isn't working, so not using this for now
        // // Load the OGG file, but wait to play it
        // sndoggvorbis_queue_enable();
        // sndoggvorbis_start(glex::targetPlatformPath(_path).c_str(), 0); // TODO: Check return code
        // _isLoaded = true;
    }

    return _isLoaded;
}

void AudioStream::unload() {
    if (!_isLoaded) return;

    if (fileType == AudioStreamFileType::MP3) {
        _shutdownMP3();
    } else if (fileType == AudioStreamFileType::OGG) {
        _shutdownOGG();
    }

    _isLoaded = false;
}

void AudioStream::play() {
    if (!_isLoaded || _isPlaying) return;

    if (fileType == AudioStreamFileType::MP3) {
        // Load and play immediately without queueing
        DEBUG_PRINTLN("starting MP3 stream at path %s", glex::targetPlatformPath(_path).c_str());
        mp3_volume(255);
        mp3_start(glex::targetPlatformPath(_path).c_str(), 0); // TODO: Check return code
    } else if (fileType == AudioStreamFileType::OGG) {
        // TODO: Queueing isn't working, so not using this for now
        // sndoggvorbis_queue_go();

        // Load and play immediately without queueing
        DEBUG_PRINTLN("starting OGG stream at path %s", glex::targetPlatformPath(_path).c_str());
        sndoggvorbis_volume(255);
        sndoggvorbis_start(glex::targetPlatformPath(_path).c_str(), 0); // TODO: Check return code
    }
    
    _isPlaying = true;
}

void AudioStream::pause() {
    // TODO: See if it's possible to pause streams
    stop();
}

void AudioStream::stop() {
    if (!_isLoaded || !_isPlaying) return;

    if (fileType == AudioStreamFileType::MP3) {
        mp3_stop();
    } else if (fileType == AudioStreamFileType::OGG) {
        sndoggvorbis_stop();
    }

    _isPlaying = false;
}

// Callback for manually decoded audio
// namespace {
//     void* _internalCallback(snd_stream_hnd_t hnd, int smp_req, int* smp_recv) {
//         DEBUG_PRINTLN("AudioStream _internalCallback hnd: %d  smp_req: %d", hnd, smp_req);
//         AudioStream* stream = (AudioStream*)snd_stream_get_userdata(hnd);
//         if (stream != NULL) {
//             stream->readBytes(smp_req, smp_recv);
//         }
//     }
// }

// Read bytes of raw audio (WIP - doesn't work right now)
// // NOTE: Right now, raw little-endian PCM files only: `ffmpeg -i song1.wav -f s16be -ar 44100 -acodec pcm_s16le song1.pcm`
// static char* tempBuffer = (char*)malloc(4096 * sizeof(char)); 
// void AudioStream::readBytes(int numberOfSamples, int* buffer) {
//     // Hard coded for 44.1KHz stereo WAV files
//     // TODO: Support other file types
//     std::streamsize size = (numberOfSamples / 16) * 2;
//     DEBUG_PRINTLN("readBytes numberOfSamples: %d  size: %d", numberOfSamples, size);
//     // char tempBuffer[size];
//     _fileStream.read(tempBuffer, size);
//     for (int i = 0; i < size; i++) {
//         buffer[i] = tempBuffer[i];
//     }
// }

// bool AudioStream::load() {
//     if (_isLoaded || _handle != SND_STREAM_INVALID) return true;

//     // Initialize the audio system if not yet initialized
//     Audio::audioInit();

//     if (_path.)

//     // Create the file handle
//     _fileStream.open(glex::targetPlatformPath(_path).c_str());
//     DEBUG_PRINTLN("AudioStream::load - _fileStream.is_open(): %d", _fileStream.is_open());

//     _handle = snd_stream_alloc(_internalCallback, 32768);
//     if (_handle == SND_STREAM_INVALID) {
//         ERROR_PRINTLN("Failed to load audio stream %s", glex::targetPlatformPath(_path).c_str());
//         _fileStream.close();
//         return false;
//     }

//     // AudioStream::_streams[_handle] = std::shared_ptr<AudioStream>(this);
//     // DEBUG_PRINTLN("_streams[%d] = %p", _handle, AudioStream::_streams[1].get());
//     snd_stream_set_userdata(_handle, this);

//     // Enable stream queuing so the music plays immediately when calling play()
//     snd_stream_queue_enable(_handle);
//     snd_stream_start(_handle, 44100, 1);

//     _isLoaded = true;
//     return _isLoaded;
// }

// void AudioStream::unload() {
//     if (!_isLoaded || _handle == SND_STREAM_INVALID) return;

//     // Are these necessary?
//     snd_stream_set_callback(_handle, NULL);
//     snd_stream_set_userdata(_handle, NULL);

//     snd_stream_destroy(_handle);
//     _fileStream.close();
//     _handle = 0;
//     _isLoaded = false;
// }

// void AudioStream::play() {
//     if (!_isLoaded || _handle == SND_STREAM_INVALID) return;

//     snd_stream_queue_go(_handle);
// }

// void AudioStream::pause() {
//     // TODO: See if it's possible to pause streams
//     stop();
// }

// void AudioStream::stop() {
//     if (!_isLoaded || _handle == SND_STREAM_INVALID) return;

//     snd_stream_stop(_handle);
// }

#endif
