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
            Layout.preferredHeight: 120
            currentOctave: root.currentOctave
            minOctave: 1
            maxOctave: 7
            onOctaveChanged: (octave) => {
                root.currentOctave = octave
                pianoKeyboard.currentOctave = octave
                // Возвращаем фокус на клавиатуру после смены октавы
                Qt.callLater(function() {
                    pianoKeyboard.forceActiveFocus()
                })
            }
        }

        // Клавиатура пианино
        PianoKeyboard {
            id: pianoKeyboard
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentOctave: root.currentOctave
            soundEngine: soundEngine
            onNoteOn: (note, velocity) => {
                console.log("Note ON:", note, "velocity:", velocity)
            }
            onNoteOff: (note) => {
                console.log("Note OFF:", note)
            }
        }
    }
}
