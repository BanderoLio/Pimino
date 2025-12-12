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

  // Проблема: loadFromModule может вызывать SIGSEGV если модуль не
  // зарегистрирован или если происходит обращение к неинициализированной памяти
  // Решение: используем прямой путь к QML файлу, который более безопасен
  //
  // В релизных версиях структура ресурсов сохраняется благодаря
  // qt_add_qml_module:
  // - Все QML файлы упаковываются в qrc:/qt/qml/Pimino/
  // - Структура директорий сохраняется: src/Main.qml ->
  // qrc:/qt/qml/Pimino/src/Main.qml
  // - Это работает как в debug, так и в release сборках
  //
  // Согласно qmldir в build/Pimino/qmldir, файлы находятся в src/Main.qml
  // Правильный путь: qrc:/qt/qml/Pimino/src/Main.qml

  QUrl qmlUrl("qrc:/qt/qml/Pimino/src/Main.qml");
  qDebug() << "Loading QML from direct path:" << qmlUrl;

  if (!qmlUrl.isValid()) {
    qCritical() << "Invalid QML URL:" << qmlUrl;
    throw std::runtime_error("Invalid QML URL");
  }

  // Проверяем существование файла в ресурсах перед загрузкой
  // Это важно для релизных версий, где ресурсы упакованы в бинарный файл
  QString resourcePath = ":/qt/qml/Pimino/src/Main.qml";
  if (!QFile::exists(resourcePath)) {
    qWarning() << "Main QML resource not found at:" << resourcePath;
    // Попробуем альтернативный путь без /src/ (на случай изменения структуры)
    resourcePath = ":/qt/qml/Pimino/Main.qml";
    if (QFile::exists(resourcePath)) {
      qmlUrl = QUrl("qrc:/qt/qml/Pimino/Main.qml");
      qDebug() << "Found QML at alternative path:" << qmlUrl;
    } else {
      qCritical() << "Main QML resource not found in any expected location";
      throw std::runtime_error(
          "Main QML resource missing - check resource compilation");
    }
  } else {
    qDebug() << "QML resource verified at:" << resourcePath;
  }

  qDebug() << "Loading QML file...";
  m_engine.load(qmlUrl);
  qDebug() << "QML load() call completed";

  qDebug() << "App::loadUI() - Checking root objects";
  if (m_engine.rootObjects().isEmpty()) {
    qCritical() << "QML was not loaded properly - no root objects";
    throw std::runtime_error("loadUI: QML was not loaded properly");
  }

  // Путь к иконке согласно структуре ресурсов: src/icons/app_icon.svg
  // В релизных версиях структура ресурсов сохраняется благодаря
  // qt_add_qml_module
  QString iconPath = ":/qt/qml/Pimino/src/icons/app_icon.svg";
  if (QFile::exists(iconPath)) {
    m_app.setWindowIcon(QIcon(iconPath));
    qDebug() << "Window icon loaded from:" << iconPath;
  } else {
    qWarning() << "Window icon not found at:" << iconPath;
    // Попробуем альтернативный путь без /src/
    iconPath = ":/qt/qml/Pimino/icons/app_icon.svg";
    if (QFile::exists(iconPath)) {
      m_app.setWindowIcon(QIcon(iconPath));
      qDebug() << "Window icon loaded from alternative path:" << iconPath;
    }
  }

  qDebug() << "App::loadUI() - QML loaded successfully, root objects:"
           << m_engine.rootObjects().size();
}

int App::exec() { return m_app.exec(); }
