import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Controls.Material
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
    property int currentTab: 0

    // Dark theme for Controls/Dialogs
    Material.theme: Material.Dark
    Material.accent: "#A7B464"

    function focusKeyboard() {
        Qt.callLater(function() {
            pianoKeyboard.forceActiveFocus()
        })
    }
    
    SoundEngineQML {
        id: soundEngine
        Component.onCompleted: {
            console.log("SoundEngine component created, initializing...")
            try {
                soundEngine.initialize()
                console.log("SoundEngine initialized")
                if (soundFontLoaded) {
                    console.log("SoundFont loaded successfully")
                } else {
                    console.warn("SoundFont not loaded. Sound will not work.")
                }
            } catch (error) {
                console.error("Error initializing SoundEngine:", error)
            }
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("Файл")
            MenuItem {
                text: qsTr("Загрузить SoundFont")
                onTriggered: soundFontDialog.open()
            }
            MenuSeparator {}
            MenuItem {
                enabled: false
                text: {
                    if (soundEngine.soundFontLoaded) {
                        let path = soundEngine.soundFontPath
                        let fileName = path.split(/[/\\]/).pop()
                        return qsTr("SoundFont: ") + (fileName || path)
                    }
                    return qsTr("SoundFont: не загружен")
                }
            }
            MenuSeparator {}
            MenuItem {
                text: qsTr("Выход")
                onTriggered: Qt.quit()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        RowLayout {
            Layout.fillWidth: true
            spacing: 12

            TabBar {
                id: tabBar
                Layout.fillWidth: true
                currentIndex: root.currentTab
                onCurrentIndexChanged: root.currentTab = currentIndex
                Material.theme: Material.Dark
                Material.accent: "#A7B464"

                TabButton { text: qsTr("Клавиатура") }
                TabButton { text: qsTr("MIDI плеер") }
            }
        }

        StackLayout {
            id: pages
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: root.currentTab

        onCurrentIndexChanged: {
            if (currentIndex === 0) {
                root.focusKeyboard()
            }
        }

            Item {
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    spacing: 12

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
                            root.focusKeyboard()
                        }
                    }

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

            // MIDI Player Page
            Rectangle {
                color: "#2a2a2a"
                radius: 8
                border.color: "#555"
                border.width: 1
                Layout.fillWidth: true
                Layout.fillHeight: true

                ColumnLayout {
                    anchors.fill: parent
                    anchors.margins: 16
                    spacing: 14

                    Text {
                        text: qsTr("MIDI плеер")
                        color: "#D4BE98"
                        font.pixelSize: 18
                        font.bold: true
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Button {
                            id: midiButton
                            text: qsTr("Выбрать MIDI")
                            onClicked: midiDialog.open()
                            background: Rectangle {
                                implicitWidth: 140
                                implicitHeight: 36
                                color: midiButton.pressed ? "#A7B464" : "#6B7A3A"
                                radius: 6
                                border.color: "#555"
                            }
                            contentItem: Text {
                                text: midiButton.text
                                color: "#D4BE98"
                                font.pixelSize: 14
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }

                        Button {
                            id: playMidiButton
                            text: soundEngine.isMidiPlaying ? qsTr("Стоп") : qsTr("Играть")
                            enabled: soundEngine.midiLoaded
                            onClicked: {
                                if (soundEngine.isMidiPlaying) {
                                    soundEngine.stopMidi()
                                } else {
                                    soundEngine.playMidi()
                                }
                                root.focusKeyboard()
                            }
                            background: Rectangle {
                                implicitWidth: 120
                                implicitHeight: 36
                                color: playMidiButton.enabled ? (playMidiButton.pressed ? "#A7B464" : "#6B7A3A") : "#3a3a3a"
                                radius: 6
                                border.color: playMidiButton.enabled ? "#555" : "#2a2a2a"
                            }
                            contentItem: Text {
                                text: playMidiButton.text
                                color: playMidiButton.enabled ? "#D4BE98" : "#666"
                                font.pixelSize: 14
                                font.bold: true
                                horizontalAlignment: Text.AlignHCenter
                                verticalAlignment: Text.AlignVCenter
                            }
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                        color: root.selectedMidiFile !== "" ? "#D4BE98" : "#888"
                        text: root.selectedMidiFile !== "" ? root.selectedMidiFile : qsTr("MIDI не выбран")
                    }

                    Text {
                        color: soundEngine.midiLoaded ? "#7FBF7F" : "#E09F3E"
                        font.pixelSize: 12
                        text: soundEngine.midiLoaded ? qsTr("Готов к воспроизведению") : qsTr("Выберите MIDI файл")
                    }

                    Item { Layout.fillHeight: true }
                }
            }
        }
    }

    FileDialog { id: soundFontDialog
        title: "Выберите SoundFont (.sf2)"
        fileMode: FileDialog.OpenFile
        nameFilters: ["SoundFont (*.sf2)", "All Files (*)"]
        onAccepted: {
            let fileUrl = selectedFile
            if (!fileUrl) {
                console.warn("No file selected")
                return
            }
            let path = fileUrl.toString().replace(/^file:\/\//, "")
            if (path.match(/^\/[A-Z]:/)) {
                path = path.substring(1)
            }
            console.log("Loading SoundFont from:", path)
            if (soundEngine.loadSoundFont(fileUrl)) {
                root.selectedSoundFont = path
            }
            Qt.callLater(function() {
                if (root.currentTab === 0) {
                    root.focusKeyboard()
                }
            })
        }
        onRejected: {
            Qt.callLater(function() {
                if (root.currentTab === 0) {
                    root.focusKeyboard()
                }
            })
        }
    }

    FileDialog {
        id: midiDialog
        title: "Выберите MIDI файл"
        fileMode: FileDialog.OpenFile
        nameFilters: ["MIDI (*.mid *.midi)", "All Files (*)"]
        onAccepted: {
            let fileUrl = selectedFile
            if (!fileUrl) {
                console.warn("No file selected")
                return
            }
            let path = fileUrl.toString().replace(/^file:\/\//, "")
            if (path.match(/^\/[A-Z]:/)) {
                path = path.substring(1)
            }
            console.log("Loading MIDI from:", path)
            if (soundEngine.loadMidiFile(fileUrl)) {
                root.selectedMidiFile = path
            }
            Qt.callLater(function() {
                if (root.currentTab === 0) {
                    root.focusKeyboard()
                }
            })
        }
        onRejected: {
            Qt.callLater(function() {
                if (root.currentTab === 0) {
                    root.focusKeyboard()
                }
            })
        }
    }
}
