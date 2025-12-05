// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause


#include <QGuiApplication>
#include <QCommandLineParser>
#include <QQmlApplicationEngine>
#include <QIcon>
#include <fluidsynth.h>
#include <thread>

// #include <core/soundengine.h>

#include "core/app.h"

int main(int argc, char *argv[])
{
    App application(argc, argv);
    // SoundEngine sEngine{};
    // sEngine.loadSoundFound("./var/")
    // 5. Play some notes
    // qDebug() << "Playing some notes...";
    // for (int i = 0; i < 12; ++i) {
    //     int key = 60 + i; // Middle C and up
    //     int velocity = 100; // A moderate velocity

    //     sEngine.noteOn(key, velocity); // Channel 0, note, velocity
    //     std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Play for 0.5 seconds
    //     sEngine.noteOff(key); // Stop the note
    //     std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Short pause between notes
    // }

    // 6. Clean up
    // qDebug() << "Cleaning up...";
    application.loadUI();
    return application.exec();
}
