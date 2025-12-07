#include "soundengineqml.h"
#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <external/fluidmidiplayer.h>

SoundEngineQML::SoundEngineQML(QObject *parent)
    : QObject(parent), m_engine(nullptr), m_soundFontLoaded(false) {
  qDebug() << "SoundEngineQML::SoundEngineQML() - Creating SoundEngine";
  try {
    m_engine = new SoundEngine();
    if (!m_engine) {
      qCritical() << "Failed to create SoundEngine";
    } else {
      qDebug() << "SoundEngineQML::SoundEngineQML() - SoundEngine created "
                  "successfully";

      QStringList possiblePaths;

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
        QFileInfo fileInfo(path);
        if (fileInfo.exists() && fileInfo.isFile()) {
          qDebug() << "Trying to load soundfont from:" << path;
          if (loadSoundFont(path)) {
            qDebug() << "Soundfont loaded successfully from:" << path;
            break;
          }
        }
      }

      if (!m_soundFontLoaded) {
        qWarning() << "No soundfont loaded. Sound will not work. "
                      "Please load a soundfont using loadSoundFont(path)";
      }
      m_engine->setMidiAutoconnect(true);
    }
  } catch (const std::exception &e) {
    qCritical() << "Exception while creating SoundEngine:" << e.what();
    m_engine = nullptr;
  } catch (...) {
    qCritical() << "Unknown exception while creating SoundEngine";
    m_engine = nullptr;
  }
}

SoundEngineQML::~SoundEngineQML() {
  if (m_engine) {
    delete m_engine;
    m_engine = nullptr;
  }
}

bool SoundEngineQML::loadSoundFont(const QString &path) {
  if (!m_engine) {
    qWarning() << "SoundEngine is not initialized";
    return false;
  }
  m_soundFontLoaded = m_engine->loadSoundFound(path.toStdString());
  if (m_soundFontLoaded) {
    qDebug() << "SoundFont loaded successfully:" << path;
  } else {
    qWarning() << "Failed to load SoundFont:" << path;
  }
  emit soundFontLoadedChanged();
  return m_soundFontLoaded;
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
