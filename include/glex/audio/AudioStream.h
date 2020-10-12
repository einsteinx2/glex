#pragma once

#include "glex/audio/Audio.h"

#ifdef DREAMCAST
#include <dc/sound/stream.h>
#include <oggvorbis/sndoggvorbis.h>
#include <memory>
#endif

#include <string>
#include <map>

/*
 * Current limitations (Dreamcast):
 *  - If using AudioStream in your KOS Dreamcast project, you must add the -ltremor and -lmp3 
 *     linker flags, even if only using one of the formats.
 *   - Only one stream can be loaded and used at once (meant for background music). However,
 *     multiple sound effects can be used at once along with background music, but they must be
 *     44.1KHz 16-bit uncompressed WAV files. They cannot be compressed. 
 *   - Only OGG Vorbis and MP3 files with a max sample rate of 44.1KHz are supported
 *   - MP3 files must be encoded using a standards compliant encoder such as BladeEnc
 *     (source code available in the `tools` directory). Encoders like LAME will not work!
 *     However, any OGG Vorbis encoder should work fine for OGG decoding.
 */

/*
 * Notes (Dreamcast):
 *   - Make sure to build the kos-ports libraries (they are already included in my Docker image).
 *   - There are mulitple OGG Vorbis decoder libraries in kos-ports, but the Tremor library is the
 *     fastest as it's written to use integer math only and optimized for KOS.
 *   - The BladeEnc MP3 encoder source code is in the `tools` directory of the project. It can be
 *     compiled on Linux, Mac, and Windows. For Mac, use the Linux instructions. It is LGPL licensed.
 */

enum AudioStreamFileType {
    MP3,
    OGG
};

class AudioStream : Audio {
public:
    AudioStreamFileType fileType;

    AudioStream(std::string path, AudioStreamFileType fileType_) : Audio(path), fileType(fileType_) {};
    ~AudioStream();

    virtual bool load();
    virtual void unload();

    virtual void play();
    virtual void pause();
    virtual void stop();

private:
    bool _isPlaying = false;
};
