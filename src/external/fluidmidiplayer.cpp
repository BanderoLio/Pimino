#include "fluidmidiplayer.h"
#include "external/fluidsynth.h"
#include <fluidsynth/midi.h>
#include <fluidsynth/misc.h>
#include <stdexcept>

FluidMidiPlayer::FluidMidiPlayer(const FluidSynth &synth)
    : m_player(new_fluid_player(synth.synth())) {}

FluidMidiPlayer::~FluidMidiPlayer() {
  fluid_player_stop(m_player);
  delete_fluid_player(m_player);
}

void FluidMidiPlayer::addMidi(const char *path) {
  if (fluid_player_add(m_player, path) == FLUID_FAILED) {
    throw new std::runtime_error("Couldn't add midi to player");
  }
}

void FluidMidiPlayer::play() {
  if (fluid_player_play(m_player) == FLUID_FAILED) {
    throw new std::runtime_error("Couldn't play midi player");
  }
}

void FluidMidiPlayer::join() { fluid_player_join(m_player); }