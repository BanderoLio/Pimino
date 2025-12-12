#ifndef APP_H
#define APP_H

#include <QCoreApplication>
#include <QGuiApplication>
#include <qguiapplication.h>
#include <qicon.h>
#include <qobject.h>
#include <qqmlapplicationengine.h>

class App {
public:
  App(QGuiApplication &);
  void loadUI(QGuiApplication &);

private:
  QQmlApplicationEngine m_engine;

  void setupQmlEngine();
};

#endif // APP_H
