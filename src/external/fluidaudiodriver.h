#ifndef FLUIDAUDIODRIVER_H
#define FLUIDAUDIODRIVER_H

#include "fluidsettings.h"
#include "fluidsynth.h"

#include <fluidsynth.h>

#include <stdexcept>

class FluidAudioDriver {
public:
  FluidAudioDriver(const FluidSettings &settings, const FluidSynth &synth);
  ~FluidAudioDriver();
  fluid_audio_driver_t *adriver() const;

private:
  fluid_audio_driver_t *m_adriver;
};

#endif // FLUIDAUDIODRIVER_H
