// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QCommandLineParser>
#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>
#include <fluidsynth.h>
#include <thread>

#include <core/soundengine.h>

#include "core/app.h"

int main(int argc, char *argv[]) {
  App application(argc, argv);
  SoundEngine sEngine{};
  // this sf can be loaded from https://www.polyphone.io/en/soundfonts/pianos/513-yamaha-cfx-studio-grand-v2
  sEngine.loadSoundFound("./var/Yamaha CFX Grand.sf2");
  qDebug() << "Playing some notes...";
  for (int i = 0; i < 12; ++i) {
    int key = 60 + i;   // Middle C and up
    int velocity = 100; // A moderate velocity

    sEngine.noteOn(key, velocity); // Channel 0, note, velocity
    std::this_thread::sleep_for(
        std::chrono::milliseconds(500)); // Play for 0.5 seconds
    sEngine.noteOff(key);                // Stop the note
    std::this_thread::sleep_for(
        std::chrono::milliseconds(100)); // Short pause between notes
  }

  qDebug() << "Cleaning up...";
  application.loadUI();
  return application.exec();
}
