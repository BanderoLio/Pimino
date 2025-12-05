#ifndef SOUNDFONTEXCEPTION_H
#define SOUNDFONTEXCEPTION_H

#include <stdexcept>


class SoundFontException : public std::runtime_error
{
public:
    SoundFontException();
};

#endif // SOUNDFONTEXCEPTION_H
