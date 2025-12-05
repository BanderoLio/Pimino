#include "core/app.h"


App::App(int argc, char* argv[]) : m_app(argc, argv), m_engine() {
    QGuiApplication::setOrganizationName("GVM");
    QGuiApplication::setApplicationName("Pimino");
    QGuiApplication::setWindowIcon(QIcon(":/qt/qml/FileSystemModule/icons/app_icon.svg"));
}

void App::loadUI() {
    m_engine.loadFromModule("FileSystemModule", "Main");
    if (m_engine.rootObjects().isEmpty())
        throw std::runtime_error("loadUI: QML was not loaded properly");
}

int App::exec() {
    return m_app.exec();
}
