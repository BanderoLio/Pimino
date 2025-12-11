// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import QtQuick.Controls.Basic

pragma ComponentBehavior: Bound

Rectangle {
    id: key
    
    property string note: "C"
    property string keyboardKey: "A"
    property bool isBlack: false
    property bool isPressed: false
    property int midiNote: 60
    property int currentOctave: 4
    property var onNoteOn: null
    property var onNoteOff: null
    required property int index
    
    readonly property string fullNoteName: {
        const octave = Math.floor(midiNote / 12) - 1
        return note + octave
    }
    
    signal pressed()
    signal released()
    
    gradient: Gradient {
        GradientStop {
            position: 0.0
            color: {
                if (isBlack) {
                    return key.isPressed ? "#0a0a0a" : (mouseArea.containsMouse ? "#1a1a1a" : "#000000")
                } else {
                    return key.isPressed ? "#d0d0d0" : (mouseArea.containsMouse ? "#f0f0f0" : "#ffffff")
                }
            }
        }
        GradientStop {
            position: 1.0
            color: {
                if (key.isBlack) {
                    return key.isPressed ? "#1a1a1a" : (mouseArea.containsMouse ? "#2a2a2a" : "#0a0a0a")
                } else {
                    return key.isPressed ? "#c0c0c0" : (mouseArea.containsMouse ? "#e8e8e8" : "#f8f8f8")
                }
            }
        }
    }
    
    border.color: isBlack ? (isPressed ? "#444" : "#222") : (isPressed ? "#999" : "#ddd")
    border.width: isPressed ? 2 : 1
    
    radius: isBlack ? 3 : 0
    
    Behavior on border.width {
        NumberAnimation { duration: 100 }
    }
    
    // Анимация нажатия
    scale: isPressed ? 0.97 : 1.0
    Behavior on scale {
        NumberAnimation { duration: 120; easing.type: Easing.OutCubic }
    }
    
    // Тень для белых клавиш
    Rectangle {
        anchors.fill: parent
        anchors.topMargin: 2
        color: "#20000000"
        radius: parent.radius
        visible: !key.isBlack && !key.isPressed
        z: -1
    }
    
    // Текст с нотами и клавишами
    Column {
        anchors.centerIn: parent
        anchors.verticalCenterOffset: key.isBlack ? -5 : 0
        spacing: 3
        visible: !key.isBlack || key.height > 50
        
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: key.fullNoteName
            color: key.isBlack ? "#fff" : "#000"
            font.pixelSize: Math.max(14, Math.min(key.height * 0.2, key.width * 0.125))
            font.bold: true
            style: Text.Outline
            styleColor: key.isBlack ? "#000" : "#fff"
            elide: Text.ElideNone
            wrapMode: Text.NoWrap
        }
        
        Text {
            anchors.horizontalCenter: parent.horizontalCenter
            text: "[" + key.keyboardKey + "]"
            color: key.isBlack ? "#bbb" : "#666"
            font.pixelSize: Math.max(10, Math.min(key.height * 0.14, key.width * 0.18))
            font.bold: false
            elide: Text.ElideNone
            wrapMode: Text.NoWrap
        }
    }
    
    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        
        onPressed: (mouse) => {
            key.isPressed = true
            key.pressed()
            if (onNoteOn) {
                onNoteOn(midiNote, 100) // velocity = 100
            }
        }
        
        onReleased: (mouse) => {
            key.isPressed = false
            key.released()
            if (onNoteOff) {
                onNoteOff(key.midiNote)
            }
        }
        
        onExited: {
            if (key.isPressed) {
                key.isPressed = false
                key.released()
                if (onNoteOff) {
                    onNoteOff(key.midiNote)
                }
            }
        }
    }
}

