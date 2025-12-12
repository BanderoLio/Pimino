#include "core/app.h"
#include "core/soundengineqml.h"
#include <QCoreApplication>
#include <QDebug>
#include <QEventLoop>
#include <QFileInfo>
#include <QMetaObject>
#include <QQmlEngine>
#include <QThread>
#include <QtQml/qqmlregistration.h>

App::App(int argc, char *argv[]) : m_app(argc, argv), m_engine() {
  // КРИТИЧНО: Устанавливаем имя приложения ДО создания QML engine
  // Это необходимо для правильной работы QML модулей
  m_app.setOrganizationName("GVM");
  m_app.setApplicationName("Pimino");

  qDebug() << "App::App() - Application initialized";
}

void App::loadUI() {
  qDebug() << "App::loadUI() - Starting QML engine initialization";

  // КРИТИЧНО: Принудительно инициализируем мета-объектную систему ПЕРЕД
  // загрузкой QML Это предотвращает SIGSEGV при создании экземпляров C++ типов
  // из QML
  const QMetaObject *metaObj = &SoundEngineQML::staticMetaObject;
  if (!metaObj) {
    qCritical() << "SoundEngineQML meta-object is null!";
    throw std::runtime_error("SoundEngineQML meta-object not initialized");
  }
  qDebug() << "SoundEngineQML meta-object initialized, class name:"
           << metaObj->className();

  // Регистрируем тип для использования в QML
  qRegisterMetaType<SoundEngineQML *>();

  // КРИТИЧНО: НЕ регистрируем тип явно, если используется QML_ELEMENT
  // Явная регистрация может конфликтовать с автоматической регистрацией через
  // QML_ELEMENT qmlRegisterType<SoundEngineQML>("Pimino", 1, 0,
  // "SoundEngineQML"); QML_ELEMENT должен автоматически регистрировать тип при
  // загрузке модуля

  // Принудительно обрабатываем все pending events для полной инициализации
  QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

  // Подключаем обработчик ошибок и предупреждений ПЕРЕД загрузкой
  QObject::connect(&m_engine, &QQmlApplicationEngine::warnings,
                   [](const QList<QQmlError> &warnings) {
                     for (const auto &warning : warnings) {
                       qWarning() << "QML Warning:" << warning.toString();
                       qWarning() << "  URL:" << warning.url()
                                  << "Line:" << warning.line();
                     }
                   });

  // Подключаем обработчик objectCreated для диагностики
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

  // Увеличиваем задержку для полной стабилизации системы (особенно важно на
  // Windows) Это критично для предотвращения race condition при инициализации
  // статических объектов
  QThread::msleep(200);
  QCoreApplication::processEvents(QEventLoop::AllEvents, 200);

  // ДИАГНОСТИКА: Проверяем import paths перед загрузкой
  qDebug() << "Qt QML engine ready, checking import paths...";
  QStringList importPaths = m_engine.importPathList();
  qDebug() << "QML import paths:" << importPaths;

  // Проверяем, что мы в главном потоке (критично для QML)
  if (QThread::currentThread() != m_app.thread()) {
    qCritical() << "loadUI() called from wrong thread!";
    throw std::runtime_error("loadUI must be called from main thread");
  }
  qDebug() << "Running in main thread, thread ID:" << QThread::currentThread();

  qDebug() << "Loading QML module...";

  // КРИТИЧНО: Согласно рекомендациям, SIGSEGV может быть из-за:
  // 1. QML файлы/модули не найдены в ресурсах
  // 2. Проблемы с threading (уже проверено)
  // 3. Отсутствующие зависимости или неправильная инициализация
  // 4. Проблемы с графическими драйверами
  //
  // Используем прямой путь к QML файлу через ресурсы
  // В qrc файле: alias="src/Main.qml", значит путь:
  // qrc:/qt/qml/Pimino/src/Main.qml

  QUrl qmlUrl("qrc:/qt/qml/Pimino/src/Main.qml");
  qDebug() << "Loading QML from direct path:" << qmlUrl;

  if (!qmlUrl.isValid()) {
    qCritical() << "Invalid QML URL:" << qmlUrl;
    throw std::runtime_error("Invalid QML URL");
  }

  // Загружаем через прямой путь (более безопасно, чем loadFromModule)
  m_engine.load(qmlUrl);

  qDebug() << "QML load() call completed";

  // Даем больше времени на асинхронную загрузку и инициализацию
  QThread::msleep(100);
  QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

  qDebug() << "App::loadUI() - Checking root objects";
  if (m_engine.rootObjects().isEmpty()) {
    qCritical() << "QML was not loaded properly - no root objects";
    throw std::runtime_error("loadUI: QML was not loaded properly");
  }

  // Устанавливаем иконку после успешной загрузки QML
  m_app.setWindowIcon(QIcon(":/qt/qml/Pimino/src/icons/app_icon.svg"));

  qDebug() << "App::loadUI() - QML loaded successfully, root objects:"
           << m_engine.rootObjects().size();
}

int App::exec() { return m_app.exec(); }
