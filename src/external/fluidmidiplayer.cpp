#include <external/fluidmidiplayer.h>
#include <external/fluidsynth.h>
#include <fluidsynth/midi.h>
#include <fluidsynth/misc.h>
#include <stdexcept>
#include <string>

FluidMidiPlayer::FluidMidiPlayer(const FluidSynth &synth)
    : m_synth(synth), m_player(new_fluid_player(synth.synth())) {}

FluidMidiPlayer::~FluidMidiPlayer() {
  fluid_player_stop(m_player);
  delete_fluid_player(m_player);
}

bool FluidMidiPlayer::loadFile(const std::string &path) {
  recreatePlayer();
  return fluid_player_add(m_player, path.c_str()) != FLUID_FAILED;
}

bool FluidMidiPlayer::play() {
  return fluid_player_play(m_player) != FLUID_FAILED;
}

void FluidMidiPlayer::stop() { fluid_player_stop(m_player); }

bool FluidMidiPlayer::isPlaying() const {
  auto status = fluid_player_get_status(m_player);
  return status == FLUID_PLAYER_PLAYING || status == FLUID_PLAYER_STOPPING;
}

void FluidMidiPlayer::wait() {
  if (isPlaying()) {
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
}