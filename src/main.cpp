// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QCommandLineParser>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <fluidsynth.h>

#include <core/soundengine.h>
#include <external/fluidmidiplayer.h>

#include "core/app.h"

int main(int argc, char *argv[]) {
  qDebug() << "main() - Starting application";
  App application(argc, argv);
  qDebug() << "main() - App created, calling loadUI()";
  application.loadUI();
  qDebug() << "main() - loadUI() completed, calling exec()";
  return application.exec();
}
