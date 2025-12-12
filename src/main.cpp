// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QCoreApplication>
#include <QDir>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <QThread>

#include "core/app.h"

// Включаем отладочные логи для QML
Q_LOGGING_CATEGORY(qml, "qt.qml")

int main(int argc, char *argv[]) {
  // Настройка логирования перед созданием приложения
  QLoggingCategory::setFilterRules("qt.qml.debug=true");

  // Настройка путей к плагинам/импортам для собранных архивов/инсталляторов
  // Делается до создания QGuiApplication
  {
    QString appDir = QDir::cleanPath(QString::fromUtf8(argv[0]));
    QFileInfo fi(appDir);
    appDir = fi.absolutePath(); // обычно .../bin

    // Плагины Qt (platforms, etc.)
    qputenv("QT_PLUGIN_PATH", QByteArray((appDir + "/../plugins").toUtf8()));
    qputenv("QT_QPA_PLATFORM_PLUGIN_PATH",
            QByteArray((appDir + "/../plugins/platforms").toUtf8()));

    // QML импорты
    qputenv("QML_IMPORT_PATH", QByteArray((appDir + "/../qml").toUtf8()));
    qputenv("QML2_IMPORT_PATH", QByteArray((appDir + "/../qml").toUtf8()));

    // Библиотеки рядом с бинарём (чтобы не подхватывались системные)
    QByteArray ldPath = QByteArray((appDir).toUtf8());
    ldPath.append(":");
    ldPath.append((appDir + "/../lib").toUtf8());
    if (qEnvironmentVariableIsSet("LD_LIBRARY_PATH")) {
      ldPath.append(":");
      ldPath.append(qgetenv("LD_LIBRARY_PATH"));
    }
    qputenv("LD_LIBRARY_PATH", ldPath);

    // Форсим xcb как более совместимый backend на Linux
    if (!qEnvironmentVariableIsSet("QT_QPA_PLATFORM")) {
      qputenv("QT_QPA_PLATFORM", QByteArray("xcb"));
    }

    // Если с драйвером OpenGL есть проблемы, можно форсить софт:
    // qputenv("QT_QUICK_BACKEND", QByteArray("software"));
  }

  qDebug() << "main() - Starting application";
  qDebug() << "main() - Qt version:" << QT_VERSION_STR;

  try {
    App application(argc, argv);
    qDebug() << "main() - App created, calling loadUI()";

    // Небольшая задержка для стабилизации системы
    QThread::msleep(50);

    application.loadUI();
    qDebug() << "main() - loadUI() completed, calling exec()";

    return application.exec();
  } catch (const std::exception &e) {
    qCritical() << "Exception in main:" << e.what();
    return 1;
  } catch (...) {
    qCritical() << "Unknown exception in main";
    return 1;
  }
}
