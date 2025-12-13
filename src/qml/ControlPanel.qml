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
    }
}

