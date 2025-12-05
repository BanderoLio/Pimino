#include "fluidsettings.h"

FluidSettings::FluidSettings() : m_settings(new_fluid_settings()) {
  if (!m_settings)
    throw std::runtime_error("Failed to create FluidSettings.");
}

FluidSettings::~FluidSettings() { delete_fluid_settings(m_settings); }

fluid_settings_t *FluidSettings::settings() const { return m_settings; }
