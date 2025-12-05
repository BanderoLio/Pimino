#include "core/app.h"

App::App(int argc, char *argv[]) : m_app(argc, argv), m_engine() {}

void App::loadUI() {
  m_engine.loadFromModule("Pimino", "Main");
  if (m_engine.rootObjects().isEmpty())
    throw std::runtime_error("loadUI: QML was not loaded properly");

  m_app.setOrganizationName("GVM");
  m_app.setApplicationName("Pimino");
  m_app.setWindowIcon(QIcon(":/qt/qml/Pimino/src/icons/app_icon.svg"));
}

int App::exec() { return m_app.exec(); }
