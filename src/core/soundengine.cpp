#include "soundengine.h"

SoundEngine::SoundEngine()
    : m_settings(), m_synth(m_settings), m_adriver(m_settings, m_synth) {}

bool SoundEngine::loadSoundFound(std::string_view path) {
  try {
    m_synth.loadSoundFound(path);
  } catch (const SoundFontException &e) {
    return false;
  }
  return true;
}

void SoundEngine::noteOn(int key, int velocity, int channel) {
  return m_synth.noteOn(key, velocity, channel);
}

void SoundEngine::noteOff(int key, int channel) {
  return m_synth.noteOff(key, channel);
}
