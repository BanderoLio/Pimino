#include "core/app.h"
#include "core/soundengineqml.h"
#include <QCoreApplication>
#include <QDebug>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QMetaObject>
#include <QQmlEngine>
#include <QThread>
#include <QtQml/qqmlregistration.h>

App::App(int argc, char *argv[]) : m_app(argc, argv), m_engine() {
  m_app.setOrganizationName("GVM");
  m_app.setApplicationName("Pimino");

  qDebug() << "App::App() - Application initialized";
}

void App::loadUI() {
  qDebug() << "App::loadUI() - Starting QML engine initialization";

  const QMetaObject *metaObj = &SoundEngineQML::staticMetaObject;
  if (!metaObj) {
    qCritical() << "SoundEngineQML meta-object is null!";
    throw std::runtime_error("SoundEngineQML meta-object not initialized");
  }
  qDebug() << "SoundEngineQML meta-object initialized, class name:"
           << metaObj->className();

  qRegisterMetaType<SoundEngineQML *>();
  QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

  QObject::connect(&m_engine, &QQmlApplicationEngine::warnings,
                   [](const QList<QQmlError> &warnings) {
                     for (const auto &warning : warnings) {
                       qWarning() << "QML Warning:" << warning.toString();
                       qWarning() << "  URL:" << warning.url()
                                  << "Line:" << warning.line();
                     }
                   });

  QObject::connect(
      &m_engine, &QQmlApplicationEngine::objectCreated, &m_app,
      [](QObject *obj, const QUrl &url) {
        if (!obj) {
          qCritical() << "Failed to create QML object from:" << url;
        } else {
          qDebug() << "Successfully created QML object from:" << url;
        }
      },
      Qt::QueuedConnection);

  qDebug() << "App::loadUI() - Loading QML via loadFromModule";

  const QString appDir = QCoreApplication::applicationDirPath();
  QStringList libPaths = QCoreApplication::libraryPaths();
  libPaths.prepend(appDir + "/plugins");
  libPaths.prepend(appDir + "/../plugins");
  QCoreApplication::setLibraryPaths(libPaths);

  m_engine.addImportPath("qrc:/");
  m_engine.addImportPath("qrc:/qt/qml");
  m_engine.addImportPath(appDir + "/qml");
  m_engine.addImportPath(appDir + "/../qml");

  qDebug() << "Qt QML engine ready, checking import paths...";
  QStringList importPaths = m_engine.importPathList();
  qDebug() << "QML import paths:" << importPaths;

  if (QThread::currentThread() != m_app.thread()) {
    qCritical() << "loadUI() called from wrong thread!";
    throw std::runtime_error("loadUI must be called from main thread");
  }
  qDebug() << "Running in main thread, thread ID:" << QThread::currentThread();

  qDebug() << "Loading QML module...";

  QUrl qmlUrl("qrc:/qt/qml/Pimino/src/Main.qml");
  qDebug() << "Loading QML from direct path:" << qmlUrl;

  if (!qmlUrl.isValid()) {
    qCritical() << "Invalid QML URL:" << qmlUrl;
    throw std::runtime_error("Invalid QML URL");
  }

  if (!QFile(":/qt/qml/Pimino/src/Main.qml").exists()) {
    qCritical() << "Main QML resource not found in resource system:" << qmlUrl;
    throw std::runtime_error("Main QML resource missing");
  }

  m_engine.load(qmlUrl);

  qDebug() << "QML load() call completed";

  qDebug() << "App::loadUI() - Checking root objects";
  if (m_engine.rootObjects().isEmpty()) {
    qCritical() << "QML was not loaded properly - no root objects";
    throw std::runtime_error("loadUI: QML was not loaded properly");
  }

  m_app.setWindowIcon(QIcon(":/qt/qml/Pimino/src/icons/app_icon.svg"));

  qDebug() << "App::loadUI() - QML loaded successfully, root objects:"
           << m_engine.rootObjects().size();
}

int App::exec() { return m_app.exec(); }
