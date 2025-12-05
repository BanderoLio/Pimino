#include "fluidaudiodriver.h"

FluidAudioDriver::FluidAudioDriver(const FluidSettings &settings,
                                   const FluidSynth &synth)
    : m_adriver(new_fluid_audio_driver(settings.settings(), synth.synth())) {
  if (!m_adriver) {
    throw std::runtime_error("Failed to create audio driver.");
  }
}

FluidAudioDriver::~FluidAudioDriver() { delete_fluid_audio_driver(m_adriver); }

fluid_audio_driver_t *FluidAudioDriver::adriver() const { return m_adriver; }
