#include "soundengine.h"
#include "external/fluidsynth.h"
#include <qdebug.h>
#include <qlogging.h>

SoundEngine::SoundEngine()
    : m_settings(), m_synth(m_settings), m_adriver(m_settings, m_synth),
      m_mdriver(m_settings, m_synth), m_isSFLoaded(false) {}

bool SoundEngine::loadSoundFound(std::string_view path) {
  try {
    m_synth.loadSoundFound(path);
    m_isSFLoaded = true;
  } catch (const SoundFontException &e) {
    return false;
  }
  return true;
}

void SoundEngine::noteOn(int key, int velocity, int channel) {
  if (!m_isSFLoaded) {
    qWarning() << "Trying to play a note without a sound font loaded";
  }
  return m_synth.noteOn(key, velocity, channel);
}

void SoundEngine::noteOff(int key, int channel) {
  return m_synth.noteOff(key, channel);
}

void SoundEngine::setMidiDriver(const char *driver) {
  fluid_settings_setstr(m_settings.settings(), "midi.driver", driver);
}

void SoundEngine::setMidiPort(const char *portname) {
  fluid_settings_setstr(m_settings.settings(), "midi.portname", portname);
}

const FluidSynth &SoundEngine::synth() { return m_synth; }