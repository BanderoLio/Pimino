import QtQuick
import QtQuick.Controls.Basic
import QtQuick.Layouts

pragma ComponentBehavior: Bound

Rectangle {
    id: controlPanel
    
    property int currentOctave: 4
    property int minOctave: 1
    property int maxOctave: 7
    property var onOctaveChanged: null
    
    color: "#2a2a2a"
    border.color: "#555"
    border.width: 1
    radius: 8
    
    ColumnLayout {
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
                text: "-"
                enabled: controlPanel.currentOctave > controlPanel.minOctave
                width: 50
                height: 50
                onClicked: {
                    if (controlPanel.currentOctave > controlPanel.minOctave) {
                        controlPanel.currentOctave--
                        if (controlPanel.onOctaveChanged) {
                            controlPanel.onOctaveChanged(controlPanel.currentOctave)
                        }
                    }
                }
                
                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#A7B464" : "#6B7A3A") : "#3a3a3a"
                    radius: 6
                    border.color: parent.enabled ? "#555" : "#2a2a2a"
                    border.width: 1
                }
                
                contentItem: Text {
                    text: parent.text
                    color: parent.enabled ? "#D4BE98" : "#666"
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
                text: "+"
                enabled: currentOctave < maxOctave
                width: 50
                height: 50
                onClicked: {
                    if (currentOctave < maxOctave) {
                        controlPanel.currentOctave++
                        if (onOctaveChanged) {
                            onOctaveChanged(controlPanel.currentOctave)
                        }
                    }
                }
                
                background: Rectangle {
                    color: parent.enabled ? (parent.pressed ? "#A7B464" : "#6B7A3A") : "#3a3a3a"
                    radius: 6
                    border.color: parent.enabled ? "#555" : "#2a2a2a"
                    border.width: 1
                }
                
                contentItem: Text {
                    text: parent.text
                    color: parent.enabled ? "#D4BE98" : "#666"
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
        
        Item {
            Layout.fillHeight: true
        }
    }
}

