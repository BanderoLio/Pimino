#ifndef FLUIDSYNTH_H
#define FLUIDSYNTH_H

#include "fluidsettings.h"

#include <string_view>

#include <stdexcept>

class FluidSynth {
public:
  FluidSynth(const FluidSettings &settings);
  ~FluidSynth();

  fluid_synth_t *synth() const;

  int gain() const;
  void setGain(float gain);

  int loadSoundFound(std::string_view path);

  void noteOn(int key, int velocity, int channel = 0);
  void noteOff(int key, int channel = 0);

private:
  FluidSettings m_settings;
  fluid_synth_t *m_synth;
  float m_gain;
};

#endif // FLUIDSYNTH_H
