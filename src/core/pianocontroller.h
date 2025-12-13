#ifndef PIANOCONTROLLER_H
#define PIANOCONTROLLER_H

#include <QMap>
#include <QObject>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

class PianoController : public QObject {
  Q_OBJECT
  // QML_ELEMENT - временно отключено для отладки

  Q_PROPERTY(int currentOctave READ currentOctave WRITE setCurrentOctave NOTIFY
                 currentOctaveChanged)
  Q_PROPERTY(int minOctave READ minOctave CONSTANT)
  Q_PROPERTY(int maxOctave READ maxOctave CONSTANT)

public:
  explicit PianoController(QObject *parent = nullptr);

  int currentOctave() const { return m_currentOctave; }
  void setCurrentOctave(int octave);

  int minOctave() const { return 1; }
  int maxOctave() const { return 7; }

  Q_INVOKABLE int noteToMidi(const QString &note, int octave);
  Q_INVOKABLE int noteToMidiWithOffset(const QString &note, int octaveOffset);
  Q_INVOKABLE QString getKeyboardKey(const QString &note, int octaveOffset);

signals:
  void currentOctaveChanged();

private:
  int m_currentOctave;
  QMap<QString, int> m_noteMap;
  QMap<QString, QMap<int, QString>> m_keyMapping;
};

#endif // PIANOCONTROLLER_H
