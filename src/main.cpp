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

  // Пути к плагинам/QML задаются через qt.conf в bin/

  qDebug() << "main() - Starting application";
  qDebug() << "main() - Qt version:" << QT_VERSION_STR;

  try {
    App application(argc, argv);
    qDebug() << "main() - App created, calling loadUI()";

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
