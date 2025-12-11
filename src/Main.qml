import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import Pimino

pragma ComponentBehavior: Bound

ApplicationWindow {
    id: root

    width: 1200
    height: 500
    minimumWidth: 800
    minimumHeight: 400
    visible: true
    color: "#292828"
    title: qsTr("Pimino - Виртуальное пианино")

    property int currentOctave: 4
    property int minOctave: 1
    property int maxOctave: 7

    function focusKeyboard() {
        Qt.callLater(function() {
            pianoKeyboard.forceActiveFocus()
        })
    }
    
    SoundEngineQML {
        id: soundEngine
        Component.onCompleted: {
            console.log("SoundEngine initialized")
            if (soundFontLoaded) {
                console.log("SoundFont loaded successfully")
            } else {
                console.warn("SoundFont not loaded. Sound will not work.")
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 20

        // Панель управления октавами
        ControlPanel {
            id: octavePanel
            Layout.fillWidth: true
            Layout.preferredHeight: octavePanel.implicitHeight
            currentOctave: root.currentOctave
            minOctave: root.minOctave
            maxOctave: root.maxOctave
            soundEngine: soundEngine
            onFocusKeyboardRequested: root.focusKeyboard()
            onOctaveChanged: (octave) => {
                root.currentOctave = octave
                pianoKeyboard.currentOctave = octave
                // Возвращаем фокус на клавиатуру после смены октавы
                root.focusKeyboard()
            }
        }

        // Клавиатура пианино
        PianoKeyboard {
            id: pianoKeyboard
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentOctave: root.currentOctave
            minOctave: root.minOctave
            maxOctave: root.maxOctave
            soundEngine: soundEngine
            onOctaveChanged: (octave) => {
                root.currentOctave = octave
            }
            Component.onCompleted: root.focusKeyboard()
            onNoteOn: (note, velocity) => {
                console.log("Note ON:", note, "velocity:", velocity)
            }
            onNoteOff: (note) => {
                console.log("Note OFF:", note)
            }
        }
    }
}
