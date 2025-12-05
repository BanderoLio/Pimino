#include "external/fluidsynth.h"
#include <fluidsynth.h>
#include <fluidsynth/types.h>

class FluidMidiPlayer {
public:
  FluidMidiPlayer(const FluidSynth &);
  ~FluidMidiPlayer();
  void addMidi(const char *path);
   void play();
   void join();
private:
  fluid_player_t *m_player;
};
