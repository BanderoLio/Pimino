#include "fluidsettings.h"

FluidSettings::FluidSettings() : m_settings(new_fluid_settings()) {
  if (!m_settings)
    throw std::runtime_error("Failed to create FluidSettings.");

  // Настройка MIDI драйвера для Linux (ALSA)
  // По умолчанию fluidsynth будет автоматически находить MIDI устройства
#ifdef __linux__
  fluid_settings_setstr(m_settings, "midi.driver", "alsa_seq");
#endif

  // Настройка полифонии (количество одновременно звучащих нот)
  // Увеличено до 512 для поддержки большого количества одновременно нажатых
  // клавиш Если нужно больше, можно увеличить до 1024 или выше
  // fluid_settings_setint(m_settings, "synth.polyphony", 512);

  // // Настройка качества звука
  // fluid_settings_setint(m_settings, "synth.sample-rate", 44100);

  // // Включение реверберации и хоруса для более реалистичного звука
  // fluid_settings_setnum(m_settings, "synth.reverb.active", 1);
  // fluid_settings_setnum(m_settings, "synth.chorus.active", 1);
}

FluidSettings::~FluidSettings() { delete_fluid_settings(m_settings); }

fluid_settings_t *FluidSettings::settings() const { return m_settings; }
