# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

android|ios {
    error( "Platform not supported" )
}

QT += quick

CONFIG += qmltypes

QML_IMPORT_PATH = $$pwd/.
QML_IMPORT_NAME = FileSystemModule
QML_IMPORT_MAJOR_VERSION = 1

TARGET = filesystemexplorer
TEMPLATE = app

SOURCES += \
    src/main.cpp \
    src/filesystemmodel.cpp \
    src/linenumbermodel.cpp \

HEADERS += \
    src/filesystemmodel.h \
    src/linenumbermodel.h \

qml_resources.files = \
    src/qmldir \
    src/Main.qml \
    src/qml/About.qml \
    src/qml/Colors.qml \
    src/qml/Editor.qml \
    src/qml/MyMenu.qml \
    src/qml/Sidebar.qml \
    src/qml/MyMenuBar.qml \
    src/qml/ResizeButton.qml \
    src/qml/FileSystemView.qml \
    src/qml/WindowDragHandler.qml \

qml_resources.prefix = /qt/qml/FileSystemModule

theme_resources.files = \
    src/icons/folder_closed.svg \
    src/icons/folder_open.svg \
    src/icons/generic_file.svg \
    src/icons/globe.svg \
    src/icons/info_sign.svg \
    src/icons/light_bulb.svg \
    src/icons/read.svg \
    src/icons/resize.svg \
    src/icons/qt_logo.svg \
    src/icons/app_icon.svg

theme_resources.prefix = /qt/qml/FileSystemModule

RESOURCES += qml_resources theme_resources

target.path = $$[QT_INSTALL_EXAMPLES]/quickcontrols/filesystemexplorer
INSTALLS += target
