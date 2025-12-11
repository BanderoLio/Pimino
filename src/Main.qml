import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs
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
    property string selectedSoundFont: ""
    property string selectedMidiFile: ""

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

        Rectangle {
            id: topBar
            Layout.fillWidth: true
            height: 56
            radius: 8
            color: "#333333"
            border.color: "#555"
            border.width: 1

            RowLayout {
                anchors.fill: parent
                anchors.margins: 10
                spacing: 10

                Button {
                    id: soundFontButton
                    text: "Загрузить SoundFont"
                    onClicked: soundFontDialog.open()
                }

                Text {
                    Layout.fillWidth: true
                    elide: Text.ElideRight
                    color: selectedSoundFont !== "" ? "#D4BE98" : "#888"
                    text: selectedSoundFont !== "" ? selectedSoundFont : "Yamaha CFX Grand.sf2 (по умолчанию)"
                }

                Button {
                    id: midiButton
                    text: "Выбрать MIDI"
                    onClicked: midiDialog.open()
                }

                Button {
                    id: playMidiButton
                    text: soundEngine.isMidiPlaying ? "Стоп" : "Играть"
                    enabled: soundEngine.midiLoaded
                    onClicked: {
                        if (soundEngine.isMidiPlaying) {
                            soundEngine.stopMidi()
                        } else {
                            soundEngine.playMidi()
                        }
                        root.focusKeyboard()
                    }
                }

                Text {
                    width: 220
                    elide: Text.ElideRight
                    color: soundEngine.midiLoaded ? "#D4BE98" : "#888"
                    text: selectedMidiFile !== "" ? selectedMidiFile : "MIDI не выбран"
                }
            }
        }

        // Панель управления октавами
        ControlPanel {
            id: octavePanel
            Layout.fillWidth: true
            Layout.preferredHeight: octavePanel.implicitHeight
            currentOctave: root.currentOctave
            minOctave: root.minOctave
            maxOctave: root.maxOctave
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

    FileDialog {
        id: soundFontDialog
        title: "Выберите SoundFont (.sf2)"
        nameFilters: ["SoundFont (*.sf2)"]
        onAccepted: {
            const path = selectedFile.toString().replace("file://", "")
            if (soundEngine.loadSoundFont(path)) {
                selectedSoundFont = path
            }
            root.focusKeyboard()
        }
        onRejected: root.focusKeyboard()
    }

    FileDialog {
        id: midiDialog
        title: "Выберите MIDI файл"
        nameFilters: ["MIDI (*.mid *.midi)"]
        onAccepted: {
            const path = selectedFile.toString().replace("file://", "")
            if (soundEngine.loadMidiFile(path)) {
                selectedMidiFile = path
            }
            root.focusKeyboard()
        }
        onRejected: root.focusKeyboard()
    }
}
