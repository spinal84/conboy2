import QtQuick 1.0
import Conboy 1.0

TextEditor {
    id: editor
    // We want the editor always at least as high as the flickable
    minHeight: flickable.height
    fontSize: 40

    onCursorPositionChanged: {
        if (rect.y + rect.height > flickable.height) {
            contentMovingAnimation.to = rect.y + rect.height - flickable.height
            contentMovingAnimation.running = true
        }
    }

    PropertyAnimation {
        id: contentMovingAnimation
        property: "contentY"
        duration: 200
        target: flickable
        easing.type: Easing.InOutCubic
    }
}
