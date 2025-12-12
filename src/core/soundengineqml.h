#ifndef SOUNDENGINEQML_H
#define SOUNDENGINEQML_H

#include "core/soundengine.h"
#include <QObject>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>
#include <memory>

class FluidMidiPlayer;

class SoundEngineQML : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(
      bool soundFontLoaded READ soundFontLoaded NOTIFY soundFontLoadedChanged)
  Q_PROPERTY(
      QString soundFontPath READ soundFontPath NOTIFY soundFontPathChanged)
  Q_PROPERTY(bool midiLoaded READ midiLoaded NOTIFY midiLoadedChanged)
  Q_PROPERTY(QString midiFilePath READ midiFilePath NOTIFY midiFilePathChanged)
  Q_PROPERTY(bool isMidiPlaying READ isMidiPlaying NOTIFY midiPlayingChanged)

public:
  explicit SoundEngineQML(QObject *parent = nullptr);
  ~SoundEngineQML();

  bool soundFontLoaded() const { return m_soundFontLoaded; }
  QString soundFontPath() const { return m_soundFontPath; }
  bool midiLoaded() const { return m_midiLoaded; }
  QString midiFilePath() const { return m_midiFilePath; }
  bool isMidiPlaying() const { return m_isMidiPlaying; }

  Q_INVOKABLE bool loadSoundFont(const QString &path);
  Q_INVOKABLE bool loadMidiFile(const QString &path);
  Q_INVOKABLE bool playMidi();
  Q_INVOKABLE void stopMidi();
  Q_INVOKABLE void noteOn(int key, int velocity = 100, int channel = 0);
  Q_INVOKABLE void noteOff(int key, int channel = 0);
  Q_INVOKABLE void initialize(); // Инициализация SoundEngine

signals:
  void soundFontLoadedChanged();
  void soundFontPathChanged();
  void midiLoadedChanged();
  void midiFilePathChanged();
  void midiPlayingChanged();

private:
  std::unique_ptr<SoundEngine> m_engine{nullptr};
  bool m_soundFontLoaded{false};
  QString m_soundFontPath;

  std::unique_ptr<FluidMidiPlayer> m_midiPlayer{nullptr};
  bool m_midiLoaded{false};
  QString m_midiFilePath;
  bool m_isMidiPlaying{false};
};

#endif // SOUNDENGINEQML_H
