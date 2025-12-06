#include "fluidmididriver.h"

static int handle_midi_event(void *data, fluid_midi_event_t *event) {
  fluid_synth_t *synth = static_cast<fluid_synth_t *>(data);
  // fluid_synth_handle_midi_event правильно обрабатывает все MIDI события,
  // включая Note On с velocity=0 как Note Off, и автоматически управляет
  // полифонией
  return fluid_synth_handle_midi_event(synth, event);
}

FluidMidiDriver::FluidMidiDriver(const FluidSettings &settings,
                                 const FluidSynth &synth)
    : m_mdriver(new_fluid_midi_driver(settings.settings(), handle_midi_event,
                                      synth.synth())) {
  if (!m_mdriver) {
    throw std::runtime_error("Failed to create MIDI driver.");
  }
}

FluidMidiDriver::~FluidMidiDriver() { delete_fluid_midi_driver(m_mdriver); }

fluid_midi_driver_t *FluidMidiDriver::mdriver() const { return m_mdriver; }
