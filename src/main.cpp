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
  App application(argc, argv);

  SoundEngine sEngine{};

  // Загрузка SoundFont
  // this sf can be loaded from
  // https://www.polyphone.io/en/soundfonts/pianos/513-yamaha-cfx-studio-grand-v2
  if (sEngine.loadSoundFound("./var/Yamaha CFX Grand.sf2")) {
    qDebug() << "SoundFont loaded successfully";
  } else {
    qWarning() << "Failed to load SoundFont. MIDI input will work, but no "
                  "sound will be produced.";
  }

  qDebug() << "========================================";
  qDebug() << "MIDI input is ENABLED and ready!";
  qDebug() << "Connect your MIDI keyboard and start playing.";
  qDebug() << "The application will listen for MIDI events.";
  qDebug() << "Press Ctrl+C or close the window to exit.";
  qDebug() << "========================================";
  FluidMidiPlayer player(sEngine.synth());
  qDebug() << "========================================";

  player.addMidi("./var/gimn.mid");
  player.addMidi("./var/pirate.mid");
  qDebug() << "========================================";
  player.play();
  qDebug() << "========================================";
  player.join();
  qDebug() << "========================================";

  // Загружаем UI и запускаем event loop
  // SoundEngine остается активным и обрабатывает MIDI события в фоне
  application.loadUI();
  return application.exec();
}
