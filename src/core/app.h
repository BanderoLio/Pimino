#ifndef APP_H
#define APP_H

#include <QGuiApplication>
#include <qicon.h>
#include <qqmlapplicationengine.h>

class App {
public:
  App(int argc, char *argv[]);
  void loadUI();
  int exec();

private:
  QGuiApplication m_app;
  QQmlApplicationEngine m_engine;
};

#endif // APP_H
