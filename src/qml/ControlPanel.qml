import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts
import QtQuick.Dialogs

pragma ComponentBehavior: Bound

Rectangle {
    id: controlPanel
    
    property int currentOctave: 4
    property int minOctave: 1
    property int maxOctave: 7
    property var soundEngine: null
    property string selectedSoundFont: ""
    property string selectedMidiFile: ""
    property var midiPlaylist: []
    property int currentMidiIndex: -1
    signal focusKeyboardRequested()
    signal octaveChanged(int octave)

    implicitHeight: panelLayout.implicitHeight + 20
    
    color: "#2a2a2a"
    border.color: "#555"
    border.width: 1
    radius: 8
    
    ColumnLayout {
        id: panelLayout
        anchors.fill: parent
        anchors.margins: 15
        spacing: 15
        
        Text {
            text: "Октава"
            color: "#D4BE98"
            font.pixelSize: 16
            font.bold: true
            Layout.alignment: Qt.AlignHCenter
        }
        
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 15
            
            Button {
                id: decrementButton
                text: "-"
                enabled: controlPanel.currentOctave > controlPanel.minOctave
                width: 50
                height: 50
                onClicked: {
                    if (controlPanel.currentOctave > controlPanel.minOctave) {
                        controlPanel.currentOctave--
                        controlPanel.octaveChanged(controlPanel.currentOctave)
                    }
                }
                
                background: Rectangle {
                    color: decrementButton.enabled ? (decrementButton.pressed ? "#A7B464" : "#6B7A3A") : "#3a3a3a"
                    radius: 6
                    border.color: decrementButton.enabled ? "#555" : "#2a2a2a"
                    border.width: 1
                }
                
                contentItem: Text {
                    text: decrementButton.text
                    color: decrementButton.enabled ? "#D4BE98" : "#666"
                    font.pixelSize: 24
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
            
            Rectangle {
                width: 80
                height: 50
                color: "#1a1a1a"
                border.color: "#555"
                border.width: 1
                radius: 6
                
                Text {
                    anchors.centerIn: parent
                    text: "C" + controlPanel.currentOctave
                    color: "#D4BE98"
                    font.pixelSize: 20
                    font.bold: true
                }
            }
            
            Button {
                id: incrementButton
                text: "+"
                enabled: controlPanel.currentOctave < controlPanel.maxOctave
                width: 50
                height: 50
                onClicked: {
                    if (controlPanel.currentOctave < controlPanel.maxOctave) {
                        controlPanel.currentOctave++
                        controlPanel.octaveChanged(controlPanel.currentOctave)
                    }
                }
                
                background: Rectangle {
                    color: incrementButton.enabled ? (incrementButton.pressed ? "#A7B464" : "#6B7A3A") : "#3a3a3a"
                    radius: 6
                    border.color: incrementButton.enabled ? "#555" : "#2a2a2a"
                    border.width: 1
                }
                
                contentItem: Text {
                    text: incrementButton.text
                    color: incrementButton.enabled ? "#D4BE98" : "#666"
                    font.pixelSize: 24
                    font.bold: true
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
            }
        }
        
        Text {
            text: "Диапазон: C" + controlPanel.minOctave + " - C" + controlPanel.maxOctave
            color: "#888"
            font.pixelSize: 11
            Layout.alignment: Qt.AlignHCenter
        }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#444"
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Text {
                text: "SoundFont"
                color: "#D4BE98"
                font.pixelSize: 14
                font.bold: true
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Button {
                    id: soundFontButton
                    text: "Выбрать .sf2"
                    onClicked: soundFontDialog.open()
                }

                Text {
                    Layout.fillWidth: true
                    wrapMode: Text.WrapAnywhere
                    color: controlPanel.selectedSoundFont !== "" ? "#D4BE98" : "#888"
                    text: controlPanel.selectedSoundFont !== "" ? controlPanel.selectedSoundFont : "Yamaha CFX Grand.sf2 (по умолчанию)"
                    elide: Text.ElideRight
                }
            }

            Text {
                color: controlPanel.soundEngine && controlPanel.soundEngine.soundFontLoaded ? "#7FBF7F" : "#E09F3E"
                font.pixelSize: 12
                text: controlPanel.soundEngine && controlPanel.soundEngine.soundFontLoaded ? "SoundFont загружен" : "SoundFont не загружен"
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 8

            Text {
                text: "MIDI плеер"
                color: "#D4BE98"
                font.pixelSize: 14
                font.bold: true
            }

            RowLayout {
                Layout.fillWidth: true
                spacing: 10

                Button {
                    id: midiButton
                    text: "Выбрать MIDI"
                    onClicked: midiDialog.open()
                }

                Text {
                    Layout.fillWidth: true
                    wrapMode: Text.WrapAnywhere
                    color: controlPanel.selectedMidiFile !== "" ? "#D4BE98" : "#888"
                    text: controlPanel.selectedMidiFile !== "" ? controlPanel.selectedMidiFile : "Файл не выбран"
                    elide: Text.ElideRight
                }
            }

            RowLayout {
                spacing: 10

                Button {
                    id: playMidiButton
                    text: controlPanel.soundEngine && controlPanel.soundEngine.isMidiPlaying ? "Стоп" : "Играть"
                    enabled: controlPanel.soundEngine && controlPanel.soundEngine.midiLoaded
                    onClicked: {
                        if (!controlPanel.soundEngine)
                            return
                        if (controlPanel.soundEngine.isMidiPlaying) {
                            controlPanel.soundEngine.stopMidi()
                        } else {
                            controlPanel.soundEngine.playMidi()
                        }
                    }
                }

                Text {
                    color: controlPanel.soundEngine && controlPanel.soundEngine.midiLoaded ? "#7FBF7F" : "#888"
                    font.pixelSize: 12
                    text: controlPanel.soundEngine && controlPanel.soundEngine.midiLoaded ? "Готов к воспроизведению" : "MIDI не выбран"
                }
            }

            RowLayout {
                spacing: 10

                Button {
                    text: "Prev"
                    enabled: controlPanel.midiPlaylist.length > 1
                    onClicked: controlPanel.loadPrevMidi()
                }

                Button {
                    text: "Next"
                    enabled: controlPanel.midiPlaylist.length > 1
                    onClicked: controlPanel.loadNextMidi()
                }

                Text {
                    Layout.fillWidth: true
                    color: controlPanel.midiPlaylist.length > 0 ? "#D4BE98" : "#888"
                    text: controlPanel.midiPlaylist.length > 0
                          ? ( (controlPanel.currentMidiIndex + 1) + "/" + controlPanel.midiPlaylist.length )
                          : "Нет плейлиста"
                    horizontalAlignment: Text.AlignRight
                }
            }
        }
        
    }

    FileDialog {
        id: soundFontDialog
        title: "Выберите SoundFont (.sf2)"
        nameFilters: ["SoundFont (*.sf2)"]
        onAccepted: {
            if (!controlPanel.soundEngine)
                return
            const path = selectedFile.toString().replace("file://", "")
            if (controlPanel.soundEngine.loadSoundFont(path)) {
                controlPanel.selectedSoundFont = path
            }
            controlPanel.focusKeyboardRequested()
        }
        onRejected: controlPanel.focusKeyboardRequested()
    }

    FileDialog {
        id: midiDialog
        title: "Выберите MIDI файл"
        nameFilters: ["MIDI (*.mid *.midi)"]
        fileMode: FileDialog.OpenFiles
        onAccepted: {
            if (!controlPanel.soundEngine)
                return
            const urls = selectedFiles && selectedFiles.length > 0 ? selectedFiles : [selectedFile]
            controlPanel.midiPlaylist = urls.map(u => u.toString().replace("file://", ""))
            controlPanel.currentMidiIndex = controlPanel.midiPlaylist.length > 0 ? 0 : -1
            controlPanel.loadCurrentMidi()
            controlPanel.focusKeyboardRequested()
        }
        onRejected: controlPanel.focusKeyboardRequested()
    }

    function loadCurrentMidi() {
        if (!controlPanel.soundEngine)
            return
        if (controlPanel.currentMidiIndex < 0 || controlPanel.currentMidiIndex >= controlPanel.midiPlaylist.length)
            return
        const path = controlPanel.midiPlaylist[controlPanel.currentMidiIndex]
        if (controlPanel.soundEngine.loadMidiFile(path)) {
            controlPanel.selectedMidiFile = path
        }
        controlPanel.focusKeyboardRequested()
    }

    function loadNextMidi() {
        if (controlPanel.midiPlaylist.length === 0)
            return
        controlPanel.currentMidiIndex = (controlPanel.currentMidiIndex + 1) % controlPanel.midiPlaylist.length
        controlPanel.loadCurrentMidi()
    }

    function loadPrevMidi() {
        if (controlPanel.midiPlaylist.length === 0)
            return
        controlPanel.currentMidiIndex = (controlPanel.currentMidiIndex - 1 + controlPanel.midiPlaylist.length) % controlPanel.midiPlaylist.length
        controlPanel.loadCurrentMidi()
    }
}

