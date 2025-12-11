#include "external/fluidsynth.h"
#include <fluidsynth.h>
#include <fluidsynth/types.h>
#include <string>

class FluidMidiPlayer {
public:
  explicit FluidMidiPlayer(const FluidSynth &);
  ~FluidMidiPlayer();

  bool loadFile(const std::string &path);
  bool play();
  void stop();
  bool isPlaying() const;
  void wait();

private:
  const FluidSynth &m_synth;
  fluid_player_t *m_player;
  void recreatePlayer();
};
