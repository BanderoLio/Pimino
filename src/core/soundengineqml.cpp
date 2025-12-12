#include "soundengineqml.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <external/fluidmidiplayer.h>

SoundEngineQML::SoundEngineQML(QObject *parent)
    : QObject(parent), m_engine(nullptr), m_soundFontLoaded(false) {
  qDebug() << "SoundEngineQML::SoundEngineQML() - Constructor called";
  // Инициализация отложена до Component.onCompleted в QML
  // Это предотвращает падение при загрузке QML
}

SoundEngineQML::~SoundEngineQML() = default;

void SoundEngineQML::initialize() {
  if (m_engine) {
    qDebug() << "SoundEngine already initialized";
    return;
  }

  qDebug() << "SoundEngineQML::initialize() - Creating SoundEngine";
  try {
    m_engine = std::make_unique<SoundEngine>();

    qDebug()
        << "SoundEngineQML::initialize() - SoundEngine created successfully";

    QStringList possiblePaths;
    QStringList attemptedPaths;

    QString appPath = QCoreApplication::applicationDirPath();
    possiblePaths << appPath + "/../var/Yamaha CFX Grand.sf2";
    possiblePaths << appPath + "/var/Yamaha CFX Grand.sf2";

    possiblePaths << "var/Yamaha CFX Grand.sf2";
    possiblePaths << QDir::currentPath() + "/var/Yamaha CFX Grand.sf2";

    possiblePaths << QStandardPaths::locate(QStandardPaths::AppDataLocation,
                                            "Yamaha CFX Grand.sf2");
    QString homeDir =
        QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    possiblePaths << homeDir + "/var/Yamaha CFX Grand.sf2";

    for (const QString &path : possiblePaths) {
      attemptedPaths << path;
      QFileInfo fileInfo(path);
      if (!fileInfo.exists() || !fileInfo.isFile()) {
        continue;
      }

      qDebug() << "Trying to load soundfont from:" << path;
      if (loadSoundFont(path)) {
        qDebug() << "Soundfont loaded successfully from:" << path;
        break;
      }
    }

    if (!m_soundFontLoaded) {
      qWarning() << "No soundfont loaded. Sound will not work.";
      qWarning() << "Soundfont search paths tried:" << attemptedPaths;
      qWarning() << "Please load a soundfont using loadSoundFont(path)";
    }

    m_midiPlayer = std::make_unique<FluidMidiPlayer>(m_engine->synth());
    m_engine->setMidiAutoconnect(true);
    qDebug() << "SoundEngineQML::initialize() - Initialization complete";
  } catch (const std::exception &e) {
    qCritical() << "Exception while creating SoundEngine:" << e.what();
    m_engine.reset();
  } catch (...) {
    qCritical() << "Unknown exception while creating SoundEngine";
    m_engine.reset();
  }
}

bool SoundEngineQML::loadSoundFont(const QString &path) {
  if (!m_engine) {
    qWarning() << "SoundEngine is not initialized";
    return false;
  }
  m_soundFontLoaded = m_engine->loadSoundFound(path.toStdString());
  if (m_soundFontLoaded) {
    m_soundFontPath = path;
    qDebug() << "SoundFont loaded successfully:" << path;
  } else {
    qWarning() << "Failed to load SoundFont:" << path;
  }
  emit soundFontLoadedChanged();
  emit soundFontPathChanged();
  return m_soundFontLoaded;
}

bool SoundEngineQML::loadMidiFile(const QString &path) {
  if (!m_engine || !m_midiPlayer) {
    qWarning() << "SoundEngine or midi player not initialized";
    return false;
  }
  m_midiPlayer->stop();
  m_midiPlayer->wait();
  m_isMidiPlaying = false;
  emit midiPlayingChanged();

  m_midiLoaded = m_midiPlayer->loadFile(path.toStdString());
  m_midiFilePath = m_midiLoaded ? path : QString();
  emit midiLoadedChanged();
  emit midiFilePathChanged();
  return m_midiLoaded;
}

bool SoundEngineQML::playMidi() {
  if (!m_midiPlayer) {
    qWarning() << "MIDI player not initialized";
    return false;
  }
  if (!m_midiLoaded) {
    qWarning() << "No MIDI file loaded";
    return false;
  }
  m_isMidiPlaying = m_midiPlayer->play();
  emit midiPlayingChanged();
  return m_isMidiPlaying;
}

void SoundEngineQML::stopMidi() {
  if (!m_midiPlayer) {
    return;
  }
  m_midiPlayer->stop();
  m_midiPlayer->wait();
  m_isMidiPlaying = false;
  emit midiPlayingChanged();
}

void SoundEngineQML::noteOn(int key, int velocity, int channel) {
  if (!m_engine) {
    qWarning() << "SoundEngine is not initialized, cannot play note";
    return;
  }
  m_engine->noteOn(key, velocity, channel);
}

void SoundEngineQML::noteOff(int key, int channel) {
  if (!m_engine) {
    return;
  }
  m_engine->noteOff(key, channel);
}
