#include <external/fluidmidiplayer.h>
#include <external/fluidsynth.h>
#include <fluidsynth/midi.h>
#include <fluidsynth/misc.h>
#include <stdexcept>
#include <string>

FluidMidiPlayer::FluidMidiPlayer(const FluidSynth &synth)
    : m_synth(synth), m_player(new_fluid_player(synth.synth())) {
  if (!m_player) {
    throw std::runtime_error("Failed to create FluidSynth MIDI player");
  }
}

FluidMidiPlayer::~FluidMidiPlayer() {
  if (m_player) {
    fluid_player_stop(m_player);
    delete_fluid_player(m_player);
    m_player = nullptr;
  }
}

bool FluidMidiPlayer::loadFile(const std::string &path) {
  recreatePlayer();
  if (!m_player) {
    return false;
  }
  return fluid_player_add(m_player, path.c_str()) != FLUID_FAILED;
}

bool FluidMidiPlayer::play() {
  if (!m_player) {
    return false;
  }
  return fluid_player_play(m_player) != FLUID_FAILED;
}

void FluidMidiPlayer::stop() {
  if (!m_player) {
    return;
  }
  fluid_player_stop(m_player);
}

bool FluidMidiPlayer::isPlaying() const {
  if (!m_player) {
    return false;
  }
  auto status = fluid_player_get_status(m_player);
  return status == FLUID_PLAYER_PLAYING || status == FLUID_PLAYER_STOPPING;
}

void FluidMidiPlayer::wait() {
  if (m_player && isPlaying()) {
    fluid_player_join(m_player);
  }
}

void FluidMidiPlayer::recreatePlayer() {
  if (m_player) {
    fluid_player_stop(m_player);
    fluid_player_join(m_player);
    delete_fluid_player(m_player);
  }
  m_player = new_fluid_player(m_synth.synth());
  if (!m_player) {
    throw std::runtime_error("Failed to recreate FluidSynth MIDI player");
  }
}