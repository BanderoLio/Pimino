# Copyright (C) 2023 The Qt Company Ltd.
# SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

cmake_minimum_required(VERSION 3.16)
project(Pimino VERSION 1.0 LANGUAGES CXX)

# Generate compile_commands.json for IDE support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (ANDROID OR IOS)
    message(FATAL_ERROR "Platform is not supported")
    return()
endif ()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Svg)

find_package(FluidSynth CONFIG REQUIRED)

qt_standard_project_setup(REQUIRES 6.8)

qt_add_executable(pimino
    src/main.cpp
    src/core/app.cpp
)

# Add src directory to include path so generated files can find headers
target_include_directories(pimino PRIVATE src)
target_include_directories(pimino PRIVATE src/core)

# Add FluidSynth include directories if needed
if(TARGET FluidSynth::libfluidsynth)
    # Modern CMake target-based approach
    # Paths are handled automatically by the target
    get_target_property(FLUIDSYNTH_INCLUDE_DIR FluidSynth::libfluidsynth INTERFACE_INCLUDE_DIRECTORIES)
    if(FLUIDSYNTH_INCLUDE_DIR)
        message(STATUS "FluidSynth include dir: ${FLUIDSYNTH_INCLUDE_DIR}")
    endif()
elseif(FLUIDSYNTH_INCLUDE_DIRS)
    target_include_directories(pimino PRIVATE ${FLUIDSYNTH_INCLUDE_DIRS})
    message(STATUS "FluidSynth include dirs: ${FLUIDSYNTH_INCLUDE_DIRS}")
elseif(FluidSynth_INCLUDE_DIRS)
    target_include_directories(pimino PRIVATE ${FluidSynth_INCLUDE_DIRS})
    message(STATUS "FluidSynth include dirs: ${FluidSynth_INCLUDE_DIRS}")
else()
    message(WARNING "FluidSynth not found. Please install it via vcpkg: vcpkg install fluidsynth")
endif()

qt_add_qml_module(pimino
    URI Pimino
    QML_FILES
        "src/Main.qml"
        "src/qml/PianoKey.qml"
        "src/qml/PianoKeyboard.qml"
        "src/qml/ControlPanel.qml"
    RESOURCES
        "src/icons/folder_closed.svg"
        "src/icons/folder_open.svg"
        "src/icons/generic_file.svg"
        "src/icons/globe.svg"
        "src/icons/info_sign.svg"
        "src/icons/light_bulb.svg"
        "src/icons/read.svg"
        "src/icons/resize.svg"
        "src/icons/qt_logo.svg"
        "src/icons/app_icon.svg"
    SOURCES
        src/filesystemmodel.cpp
        src/filesystemmodel.h
        src/linenumbermodel.cpp
        src/linenumbermodel.h
        src/core/app.h
        src/core/soundengineqml.h
        src/core/soundengineqml.cpp
        src/core/pianocontroller.cpp
        src/core/pianocontroller.h
        src/external/fluidsettings.h src/external/fluidsettings.cpp
        src/external/fluidsynth.h src/external/fluidsynth.cpp
        src/external/fluidaudiodriver.h src/external/fluidaudiodriver.cpp
        src/external/fluidmididriver.h src/external/fluidmididriver.cpp
        src/external/fluidmidiplayer.h src/external/fluidmidiplayer.cpp
        src/core/soundengine.h src/core/soundengine.cpp
        SOURCES src/external/exceptions/soundfontexception.h src/external/exceptions/soundfontexception.cpp
)

target_link_libraries(pimino
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Quick
        Qt6::QuickControls2
        Qt6::Svg
        FluidSynth::libfluidsynth
)

set_target_properties(pimino
    PROPERTIES
        WIN32_EXECUTABLE TRUE
)

if (APPLE)
    set_target_properties(pimino PROPERTIES
        OUTPUT_NAME "File System Explorer"
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_BUNDLE_NAME "File System Explorer"
        MACOSX_BUNDLE_COPYRIGHT "Copyright (C) The Qt Company Ltd. and other contributors."
        MACOSX_BUNDLE_GUI_IDENTIFIER "io.qt.examples.fsexplorer"
        MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
        MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    )
endif()

install(TARGETS pimino
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_qml_app_script(
    TARGET pimino
    OUTPUT_SCRIPT deploy_script
    MACOS_BUNDLE_POST_BUILD
    NO_UNSUPPORTED_PLATFORM_ERROR
    DEPLOY_USER_QML_MODULES_ON_UNSUPPORTED_PLATFORM
)
install(SCRIPT ${deploy_script})
