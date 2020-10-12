#pragma once

#include <string>

class Audio {
public:
    // static void audioInit();

    virtual ~Audio() {}

    virtual bool load() = 0;
    virtual void unload() = 0;

    virtual void play() = 0;
    virtual void pause() = 0;
    virtual void stop() = 0;

protected:
    std::string _path = "";
    bool _isLoaded = false;
    // bool _isPlaying = false;

    Audio(std::string path) : _path(path) {}

private:
    static bool _isAudioInitialized;
};
