#ifndef FLUIDSETTINGS_H
#define FLUIDSETTINGS_H

#include <fluidsynth.h>

#include <stdexcept>

class FluidSettings
{
public:
    FluidSettings();
    ~FluidSettings();
    fluid_settings_t* settings() const;
private:
    fluid_settings_t* m_settings;
};

#endif // FLUIDSETTINGS_H
