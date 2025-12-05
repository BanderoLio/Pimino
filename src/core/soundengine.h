#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <external/fluidaudiodriver.h>
#include <external/fluidsynth.h>
#include <external/exceptions/soundfontexception.h>

class SoundEngine
{
public:
    SoundEngine();
    bool loadSoundFound(std::string_view path);
    void noteOn(int key, int velocity, int channel = 0);
    void noteOff(int key, int channel = 0);
private:
    FluidSettings m_settings;
    FluidSynth m_synth;
    FluidAudioDriver m_adriver;
};

#endif // SOUNDENGINE_H
