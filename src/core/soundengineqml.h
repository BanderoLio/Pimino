#ifndef SOUNDENGINEQML_H
#define SOUNDENGINEQML_H

#include "core/soundengine.h"
#include <QObject>
#include <QString>
#include <QtQmlIntegration/qqmlintegration.h>

class SoundEngineQML : public QObject {
  Q_OBJECT
  QML_ELEMENT

  Q_PROPERTY(
      bool soundFontLoaded READ soundFontLoaded NOTIFY soundFontLoadedChanged)

public:
  explicit SoundEngineQML(QObject *parent = nullptr);
  ~SoundEngineQML();

  bool soundFontLoaded() const { return m_soundFontLoaded; }

  Q_INVOKABLE bool loadSoundFont(const QString &path);
  Q_INVOKABLE void noteOn(int key, int velocity = 100, int channel = 0);
  Q_INVOKABLE void noteOff(int key, int channel = 0);

signals:
  void soundFontLoadedChanged();

private:
  SoundEngine *m_engine;
  bool m_soundFontLoaded;
};

#endif // SOUNDENGINEQML_H
