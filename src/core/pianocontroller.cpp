#include "pianocontroller.h"
#include <QDebug>

PianoController::PianoController(QObject *parent)
    : QObject(parent), m_currentOctave(4) {
  // Инициализация маппинга нот в MIDI
  m_noteMap["C"] = 0;
  m_noteMap["C#"] = 1;
  m_noteMap["D"] = 2;
  m_noteMap["D#"] = 3;
  m_noteMap["E"] = 4;
  m_noteMap["F"] = 5;
  m_noteMap["F#"] = 6;
  m_noteMap["G"] = 7;
  m_noteMap["G#"] = 8;
  m_noteMap["A"] = 9;
  m_noteMap["A#"] = 10;
  m_noteMap["B"] = 11;

  // Маппинг клавиш QWERTY на ноты (2 октавы)
  // Первая октава (octaveOffset = 0)
  m_keyMapping["C"][0] = "A";
  m_keyMapping["C#"][0] = "W";
  m_keyMapping["D"][0] = "S";
  m_keyMapping["D#"][0] = "E";
  m_keyMapping["E"][0] = "D";
  m_keyMapping["F"][0] = "F";
  m_keyMapping["F#"][0] = "T";
  m_keyMapping["G"][0] = "G";
  m_keyMapping["G#"][0] = "Y";
  m_keyMapping["A"][0] = "H";
  m_keyMapping["A#"][0] = "U";
  m_keyMapping["B"][0] = "J";

  // Вторая октава (octaveOffset = 1)
  m_keyMapping["C"][1] = "K";
  m_keyMapping["C#"][1] = "O";
  m_keyMapping["D"][1] = "L";
  m_keyMapping["D#"][1] = "P";
  m_keyMapping["E"][1] = ";";
  m_keyMapping["F"][1] = "'";
  m_keyMapping["F#"][1] = "]";
  m_keyMapping["G"][1] = "\\";
  m_keyMapping["G#"][1] = "Z";
  m_keyMapping["A"][1] = "X";
  m_keyMapping["A#"][1] = "C";
  m_keyMapping["B"][1] = "V";
}

void PianoController::setCurrentOctave(int octave) {
  if (octave >= minOctave() && octave <= maxOctave() &&
      octave != m_currentOctave) {
    m_currentOctave = octave;
    emit currentOctaveChanged();
  }
}

int PianoController::noteToMidi(const QString &note, int octave) {
  if (!m_noteMap.contains(note)) {
    qWarning() << "Unknown note:" << note;
    return 60; // C4 по умолчанию
  }
  return (octave + 1) * 12 + m_noteMap[note];
}

int PianoController::noteToMidiWithOffset(const QString &note,
                                          int octaveOffset) {
  return noteToMidi(note, m_currentOctave + octaveOffset);
}

QString PianoController::getKeyboardKey(const QString &note, int octaveOffset) {
  if (m_keyMapping.contains(note) &&
      m_keyMapping[note].contains(octaveOffset)) {
    return m_keyMapping[note][octaveOffset];
  }
  return "";
}
