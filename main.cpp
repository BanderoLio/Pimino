// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QIcon>

int main(int argc, char *argv[])
{
    // Initialize the static application object.
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("GVM");
    QGuiApplication::setApplicationName("Pimino");
    // TODO: change icon
    QGuiApplication::setWindowIcon(QIcon(":/qt/qml/FileSystemModule/icons/app_icon.svg"));

    // Load the QML entry point.
    QQmlApplicationEngine engine;
    engine.loadFromModule("FileSystemModule", "Main");
    if (engine.rootObjects().isEmpty())
        return -1;

    return QGuiApplication::exec(); // Start the event loop.
}
