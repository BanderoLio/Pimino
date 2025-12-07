#include "fluidsettings.h"
#include <fluidsynth/settings.h>

FluidSettings::FluidSettings() : m_settings(new_fluid_settings()) {
  if (!m_settings)
    throw std::runtime_error("Failed to create FluidSettings.");

#ifdef __linux__
  fluid_settings_setstr(m_settings, "midi.driver", "alsa_seq");
  fluid_settings_setstr(m_settings, "audio.driver", "alsa");
  fluid_settings_setstr(m_settings, "audio.alsa.device", "default");
  fluid_settings_setint(m_settings, "midi.autoconnect", 1);

#endif

  fluid_settings_setint(m_settings, "synth.polyphony", 256);

  fluid_settings_setint(m_settings, "synth.sample-rate", 44100);

  // Настройка аудио буфера для предотвращения пропадания звука
  // period-size: размер одного периода в сэмплах (меньше = меньше задержка, но
  // больше нагрузка) periods: количество периодов в буфере (больше = более
  // стабильно, но больше задержка) Для стабильной работы при большой нагрузке
  // используем больший буфер Увеличенный буфер поможет избежать пропадания
  // звука при быстрой игре
  fluid_settings_setint(m_settings, "audio.period-size", 64);
  fluid_settings_setint(m_settings, "audio.periods", 16);

  fluid_settings_setnum(m_settings, "synth.reverb.active", 1);
  fluid_settings_setnum(m_settings, "synth.chorus.active", 1);

  fluid_settings_setint(m_settings, "audio.realtime-prio", 60);
}

FluidSettings::~FluidSettings() { delete_fluid_settings(m_settings); }

fluid_settings_t *FluidSettings::settings() const { return m_settings; }
