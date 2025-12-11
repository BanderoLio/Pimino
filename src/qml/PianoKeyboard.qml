// Copyright (C) 2023 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

import QtQuick
import Pimino

pragma ComponentBehavior: Bound

FocusScope {
    id: keyboard
    
    property int currentOctave: 4 // C4 по умолчанию (средняя октава)
    property int minOctave: 1
    property int maxOctave: 7
    property var controller: null
    property var soundEngine: null

    signal noteOn(int note, int velocity)
    signal noteOff(int note)
    signal octaveChanged(int octave)
    
    // Обработка изменения октавы - останавливаем все текущие ноты
    onCurrentOctaveChanged: {
        // Останавливаем все активные MIDI ноты
        if (soundEngine) {
            for (let i = 0; i < activeMidiNotes.length; i++) {
                soundEngine.noteOff(activeMidiNotes[i])
            }
        }
        
        // Останавливаем все нажатые клавиши визуально
        const keysToRelease = Object.keys(pressedKeys)
        for (let i = 0; i < keysToRelease.length; i++) {
            const key = keysToRelease[i]
            const keyInfo = keyOrder.find(k => k.keyboardKey === key)
            if (keyInfo) {
                updateKeyVisualState(keyInfo, false)
            }
        }
        
        // Очищаем списки - создаем новые объекты/массивы
        pressedKeys = {}
        activeMidiNotes = []
        
        // Восстанавливаем фокус для обработки клавиатуры через Qt.callLater
        // чтобы это произошло после завершения обработки события смены октавы
        Qt.callLater(function() {
            keyboard.forceActiveFocus()
        })
    }
    
    readonly property var keyOrder: [
        {note: "C", isBlack: false, keyboardKey: "A", octaveOffset: 0},
        {note: "C#", isBlack: true, keyboardKey: "W", octaveOffset: 0},
        {note: "D", isBlack: false, keyboardKey: "S", octaveOffset: 0},
        {note: "D#", isBlack: true, keyboardKey: "E", octaveOffset: 0},
        {note: "E", isBlack: false, keyboardKey: "D", octaveOffset: 0},
        {note: "F", isBlack: false, keyboardKey: "F", octaveOffset: 0},
        {note: "F#", isBlack: true, keyboardKey: "T", octaveOffset: 0},
        {note: "G", isBlack: false, keyboardKey: "G", octaveOffset: 0},
        {note: "G#", isBlack: true, keyboardKey: "Y", octaveOffset: 0},
        {note: "A", isBlack: false, keyboardKey: "H", octaveOffset: 0},
        {note: "A#", isBlack: true, keyboardKey: "U", octaveOffset: 0},
        {note: "B", isBlack: false, keyboardKey: "J", octaveOffset: 0},
        {note: "C", isBlack: false, keyboardKey: "K", octaveOffset: 1}, // Следующая октава
        {note: "C#", isBlack: true, keyboardKey: "O", octaveOffset: 1},
        {note: "D", isBlack: false, keyboardKey: "L", octaveOffset: 1},
        {note: "D#", isBlack: true, keyboardKey: "P", octaveOffset: 1},
        {note: "E", isBlack: false, keyboardKey: ";", octaveOffset: 1},
        {note: "F", isBlack: false, keyboardKey: "'", octaveOffset: 1},
        {note: "F#", isBlack: true, keyboardKey: "]", octaveOffset: 1},
        {note: "G", isBlack: false, keyboardKey: "\\", octaveOffset: 1},
        {note: "G#", isBlack: true, keyboardKey: "Z", octaveOffset: 1},
        {note: "A", isBlack: false, keyboardKey: "X", octaveOffset: 1},
        {note: "A#", isBlack: true, keyboardKey: "C", octaveOffset: 1},
        {note: "B", isBlack: false, keyboardKey: "V", octaveOffset: 1}
    ]
    
    readonly property var noteMap: {
        "C": 0, "C#": 1, "D": 2, "D#": 3, "E": 4, "F": 5,
        "F#": 6, "G": 7, "G#": 8, "A": 9, "A#": 10, "B": 11
    }
    
    function noteToMidi(note, octaveOffset) {
        const noteOffset = noteMap[note] || 0
        const octave = currentOctave + octaveOffset
        return (octave + 1) * 12 + noteOffset
    }
    
    function getWhiteKey(index) {
        let whiteIndex = 0
        for (let i = 0; i < keyOrder.length; i++) {
            if (!keyOrder[i].isBlack) {
                if (whiteIndex === index) {
                    return keyOrder[i]
                }
                whiteIndex++
            }
        }
        return null
    }
    
    function getBlackKey(index) {
        let blackIndex = 0
        for (let i = 0; i < keyOrder.length; i++) {
            if (keyOrder[i].isBlack) {
                if (blackIndex === index) {
                    return keyOrder[i]
                }
                blackIndex++
            }
        }
        return null
    }
    
    property var pressedKeys: ({})
    property var activeMidiNotes: [] // Храним активные MIDI ноты для правильной остановки при смене октавы
    
    function handleKeyPress(keyText) {
        const keyUpper = keyText.toUpperCase()
        // Убеждаемся, что pressedKeys инициализирован
        if (!pressedKeys || typeof pressedKeys !== 'object') {
            pressedKeys = {}
        }
        if (pressedKeys[keyUpper]) {
            return
        }
        
        const keyInfo = keyOrder.find(k => k.keyboardKey === keyUpper)
        if (keyInfo) {
            pressedKeys[keyUpper] = true
            const midiNote = noteToMidi(keyInfo.note, keyInfo.octaveOffset)
            // Убеждаемся, что activeMidiNotes инициализирован
            if (!activeMidiNotes || !Array.isArray(activeMidiNotes)) {
                activeMidiNotes = []
            }
            // Добавляем MIDI ноту в список активных
            if (activeMidiNotes.indexOf(midiNote) === -1) {
                activeMidiNotes.push(midiNote)
            }
            if (soundEngine) {
                soundEngine.noteOn(midiNote, 100)
            }
            keyboard.noteOn(midiNote, 100)
            updateKeyVisualState(keyInfo, true)
        }
    }
    
    function handleKeyRelease(keyText) {
        const keyUpper = keyText.toUpperCase()
        if (!pressedKeys || !pressedKeys[keyUpper]) {
            return
        }
        
        const keyInfo = keyOrder.find(k => k.keyboardKey === keyUpper)
        if (keyInfo) {
            delete pressedKeys[keyUpper]
            const midiNote = noteToMidi(keyInfo.note, keyInfo.octaveOffset)
            // Удаляем MIDI ноту из списка активных
            if (activeMidiNotes && Array.isArray(activeMidiNotes)) {
                const noteIndex = activeMidiNotes.indexOf(midiNote)
                if (noteIndex !== -1) {
                    activeMidiNotes.splice(noteIndex, 1)
                }
            }
            if (soundEngine) {
                soundEngine.noteOff(midiNote)
            }
            keyboard.noteOff(midiNote)
            updateKeyVisualState(keyInfo, false)
        }
    }
    
    function updateKeyVisualState(keyInfo, pressed) {
        if (keyInfo.isBlack) {
            const blackKeys = keyOrder.filter(k => k.isBlack)
            const blackIndex = blackKeys.findIndex(k => 
                k.note === keyInfo.note && 
                k.keyboardKey === keyInfo.keyboardKey &&
                k.octaveOffset === keyInfo.octaveOffset
            )
            if (blackIndex >= 0 && blackKeysRepeater.itemAt(blackIndex)) {
                blackKeysRepeater.itemAt(blackIndex).isPressed = pressed
            }
        } else {
            const whiteKeys = keyOrder.filter(k => !k.isBlack)
            const whiteIndex = whiteKeys.findIndex(k => 
                k.note === keyInfo.note && 
                k.keyboardKey === keyInfo.keyboardKey &&
                k.octaveOffset === keyInfo.octaveOffset
            )
            if (whiteIndex >= 0 && whiteKeysRepeater.itemAt(whiteIndex)) {
                whiteKeysRepeater.itemAt(whiteIndex).isPressed = pressed
            }
        }
    }
    
    function keyToString(key) {
        if (key === Qt.Key_A) return "A"
        if (key === Qt.Key_B) return "B"
        if (key === Qt.Key_C) return "C"
        if (key === Qt.Key_D) return "D"
        if (key === Qt.Key_E) return "E"
        if (key === Qt.Key_F) return "F"
        if (key === Qt.Key_G) return "G"
        if (key === Qt.Key_H) return "H"
        if (key === Qt.Key_I) return "I"
        if (key === Qt.Key_J) return "J"
        if (key === Qt.Key_K) return "K"
        if (key === Qt.Key_L) return "L"
        if (key === Qt.Key_M) return "M"
        if (key === Qt.Key_N) return "N"
        if (key === Qt.Key_O) return "O"
        if (key === Qt.Key_P) return "P"
        if (key === Qt.Key_Q) return "Q"
        if (key === Qt.Key_R) return "R"
        if (key === Qt.Key_S) return "S"
        if (key === Qt.Key_T) return "T"
        if (key === Qt.Key_U) return "U"
        if (key === Qt.Key_V) return "V"
        if (key === Qt.Key_W) return "W"
        if (key === Qt.Key_X) return "X"
        if (key === Qt.Key_Y) return "Y"
        if (key === Qt.Key_Z) return "Z"
        if (key === Qt.Key_Semicolon) return ";"
        if (key === Qt.Key_Apostrophe) return "'"
        if (key === Qt.Key_BracketRight) return "]"
        if (key === Qt.Key_Backslash) return "\\"
        return ""
    }

    
    // Фокус для обработки событий клавиатуры
    focus: true
    Keys.forwardTo: []
    Component.onCompleted: keyboard.forceActiveFocus()
    
    Keys.onPressed: (event) => {
        if (!event.isAutoRepeat) {
            if (event.key === Qt.Key_Plus || event.key === Qt.Key_Equal) {
                const nextOctave = Math.min(maxOctave, currentOctave + 1)
                if (nextOctave !== currentOctave) {
                    keyboard.octaveChanged(nextOctave)
                }
                event.accepted = true
                return
            }
            if (event.key === Qt.Key_Minus) {
                const prevOctave = Math.max(minOctave, currentOctave - 1)
                if (prevOctave !== currentOctave) {
                    keyboard.octaveChanged(prevOctave)
                }
                event.accepted = true
                return
            }
            let keyText = keyToString(event.key)
            if (keyText) {
                handleKeyPress(keyText)
                event.accepted = true
            }
        }
    }
    
    Keys.onReleased: (event) => {
        if (!event.isAutoRepeat) {
            let keyText = keyToString(event.key)
            if (keyText) {
                handleKeyRelease(keyText)
                event.accepted = true
            }
        }
    }
    
    Row {
        id: whiteKeysRow
        anchors.fill: parent
        spacing: 0
        
        Repeater {
            id: whiteKeysRepeater
            model: 15
            
            delegate: PianoKey {
                id: whiteKeyItem
                width: keyboard.width / 15
                height: keyboard.height
                
                property var keyData: keyboard.getWhiteKey(index)
                
                note: keyData ? keyData.note : "C"
                keyboardKey: keyData ? keyData.keyboardKey : "A"
                isBlack: false
                currentOctave: keyboard.currentOctave
                midiNote: {
                    if (keyData) {
                        return keyboard.noteToMidi(keyData.note, keyData.octaveOffset)
                    }
                    return 60
                }
                onNoteOn: (note, vel) => {
                    // Добавляем MIDI ноту в список активных
                    if (keyboard.activeMidiNotes.indexOf(note) === -1) {
                        keyboard.activeMidiNotes.push(note)
                    }
                    if (keyboard.soundEngine) {
                        keyboard.soundEngine.noteOn(note, vel)
                    }
                    keyboard.noteOn(note, vel)
                }
                onNoteOff: (note) => {
                    // Удаляем MIDI ноту из списка активных
                    const noteIndex = keyboard.activeMidiNotes.indexOf(note)
                    if (noteIndex !== -1) {
                        keyboard.activeMidiNotes.splice(noteIndex, 1)
                    }
                    if (keyboard.soundEngine) {
                        keyboard.soundEngine.noteOff(note)
                    }
                    keyboard.noteOff(note)
                }
                z: 0
            }
        }
    }
    
    Item {
        id: blackKeysContainer
        anchors.fill: parent
        z: 1
        
        Repeater {
            id: blackKeysRepeater
            model: 10
            
            delegate: PianoKey {
                id: blackKeyItem
                width: keyboard.width / 15 * 0.6
                height: keyboard.height * 0.4
                
                property var keyData: keyboard.getBlackKey(index)
                
                property int fullIndex: {
                    if (!keyData) return -1
                    for (let i = 0; i < keyboard.keyOrder.length; i++) {
                        if (keyboard.keyOrder[i].note === keyData.note && 
                            keyboard.keyOrder[i].isBlack && 
                            keyboard.keyOrder[i].keyboardKey === keyData.keyboardKey &&
                            keyboard.keyOrder[i].octaveOffset === keyData.octaveOffset) {
                            return i
                        }
                    }
                    return -1
                }
                
                note: keyData ? keyData.note : "C#"
                keyboardKey: keyData ? keyData.keyboardKey : "W"
                isBlack: true
                currentOctave: keyboard.currentOctave
                midiNote: {
                    if (keyData) {
                        return keyboard.noteToMidi(keyData.note, keyData.octaveOffset)
                    }
                    return 61
                }
                onNoteOn: (note, vel) => {
                    // Добавляем MIDI ноту в список активных
                    if (keyboard.activeMidiNotes.indexOf(note) === -1) {
                        keyboard.activeMidiNotes.push(note)
                    }
                    if (keyboard.soundEngine) {
                        keyboard.soundEngine.noteOn(note, vel)
                    }
                    keyboard.noteOn(note, vel)
                }
                onNoteOff: (note) => {
                    // Удаляем MIDI ноту из списка активных
                    const noteIndex = keyboard.activeMidiNotes.indexOf(note)
                    if (noteIndex !== -1) {
                        keyboard.activeMidiNotes.splice(noteIndex, 1)
                    }
                    if (keyboard.soundEngine) {
                        keyboard.soundEngine.noteOff(note)
                    }
                    keyboard.noteOff(note)
                }
                
                x: {
                    if (fullIndex < 0) return 0
                    
                    let whiteKeyIndex = -1
                    for (let i = 0; i < fullIndex; i++) {
                        if (!keyboard.keyOrder[i].isBlack) {
                            whiteKeyIndex++
                        }
                    }
                    
                    const whiteKeyWidth = keyboard.width / 15
                    return (whiteKeyIndex * whiteKeyWidth) + (whiteKeyWidth * 0.65)
                }
            }
        }
    }
}

