#ifndef FLUIDMIDIDRIVER_H
#define FLUIDMIDIDRIVER_H

#include "fluidsettings.h"
#include "fluidsynth.h"

#include <fluidsynth.h>

#include <stdexcept>

class FluidMidiDriver {
public:
  FluidMidiDriver(const FluidSettings &settings, const FluidSynth &synth);
  ~FluidMidiDriver();
  fluid_midi_driver_t *mdriver() const;

private:
  fluid_midi_driver_t *m_mdriver;
};

#endif // FLUIDMIDIDRIVER_H

