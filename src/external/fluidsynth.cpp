#include "fluidsynth.h"

#include <external/exceptions/soundfontexception.h>
#include <fluidsynth/synth.h>

FluidSynth::FluidSynth(const FluidSettings &settings)
    : m_settings(settings), m_synth(new_fluid_synth(settings.settings())) {
  if (!m_synth)
    throw std::runtime_error("Failed to create FluidSynth synthesizer.");
  fluid_synth_set_gain(synth(), 3.0f);
}

FluidSynth::~FluidSynth() { delete_fluid_synth(m_synth); }

fluid_synth_t *FluidSynth::synth() const { return m_synth; }

int FluidSynth::loadSoundFound(std::string_view path) {
  // 1 - reset presets
  auto id = fluid_synth_sfload(synth(), path.data(), 1);
  if (id == FLUID_FAILED) {
    throw SoundFontException();
  }
  return id;
}

void FluidSynth::noteOn(int key, int velocity, int channel) {
  fluid_synth_noteon(m_synth, channel, key, velocity);
}
void FluidSynth::noteOff(int key, int channel) {
  fluid_synth_noteoff(m_synth, channel, key);
}
