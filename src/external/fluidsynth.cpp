#include "fluidsynth.h"

#include <external/exceptions/soundfontexception.h>
#include <fluidsynth/synth.h>
#include <qdebug.h>
#include <qlogging.h>

FluidSynth::FluidSynth(const FluidSettings &settings)
    : m_settings(settings), m_synth(new_fluid_synth(settings.settings())) {
  if (!m_synth) {
    throw std::runtime_error("Failed to create FluidSynth synthesizer.");
  }
  setGain(1.0f);
}

FluidSynth::~FluidSynth() {
  if (m_synth) {
    delete_fluid_synth(m_synth);
    m_synth = nullptr;
  }
}

fluid_synth_t *FluidSynth::synth() const { return m_synth; }

int FluidSynth::gain() const { return m_gain; }

void FluidSynth::setGain(float gain) {
  if (!m_synth) {
    return;
  }
  if (gain < .0f || gain > 5.0f) {
    qWarning() << "setGain: value probably can cause problems";
  }
  fluid_synth_set_gain(m_synth, gain);
  m_gain = gain;
}

int FluidSynth::loadSoundFound(std::string_view path) {
  // 1 - reset presets
  if (!m_synth) {
    throw std::runtime_error("FluidSynth is not initialized");
  }
  auto id = fluid_synth_sfload(synth(), path.data(), 1);
  if (id == FLUID_FAILED) {
    throw SoundFontException();
  }
  return id;
}

void FluidSynth::noteOn(int key, int velocity, int channel) {
  if (!m_synth) {
    return;
  }
  fluid_synth_noteon(m_synth, channel, key, velocity);
}
void FluidSynth::noteOff(int key, int channel) {
  if (!m_synth) {
    return;
  }
  fluid_synth_noteoff(m_synth, channel, key);
}
