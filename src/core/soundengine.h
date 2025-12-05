#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include <external/exceptions/soundfontexception.h>
#include <external/fluidaudiodriver.h>
#include <external/fluidmididriver.h>
#include <external/fluidsynth.h>

class SoundEngine {
public:
  SoundEngine();
  bool loadSoundFound(std::string_view path);
  void noteOn(int key, int velocity, int channel = 0);
  void noteOff(int key, int channel = 0);
  void setMidiDriver(const char *driver);
  void setMidiPort(const char *portname);

  // TODO FIXME remove, test purpuses
  const FluidSynth& synth();

private:
  FluidSettings m_settings;
  FluidSynth m_synth;
  FluidAudioDriver m_adriver;
  FluidMidiDriver m_mdriver;
  bool m_isSFLoaded{};
};

#endif // SOUNDENGINE_H
