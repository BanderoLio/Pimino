#include "soundfontexception.h"

SoundFontException::SoundFontException()
    : std::runtime_error("Sound font couldn't be loaded") {}
