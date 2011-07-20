import QtQuick 1.0
import Conboy 1.0

TextEditor {
    id: editor
    fontSize: 40

    // TODO: Should only scroll if cursor is near widget borders
    onCursorPositionChanged: {
        var flick = findFlickable(parent)
        if (rect.y + rect.height > flick.height) {
            var newContentY = rect.y + rect.height - flick.height

            // If overpanned, set contentY to max possible value (reached bottom)
            // Not sure we need that
            if (newContentY > flick.contentHeight - flick.height) {
                newContentY = flick.contentHeight - flick.height
            }

            // If overpanned, set contentY to min possible value (reached top)
            // Not sure we need that
            if (newContentY < 0) {
                newContentY = 0
            }

            if (newContentY != flick.contentY) {
                contentMovingAnimation.target = flick
                contentMovingAnimation.to = newContentY
                contentMovingAnimation.running = true
            }
        }
    }

    PropertyAnimation {
        id: contentMovingAnimation
        property: "contentY"
        duration: 200
        easing.type: Easing.InOutCubic
    }

    function findFlickable(component) {
        var nextParent = component
        while(nextParent) {
            if(nextParent.flicking !== undefined && nextParent.flickableDirection !== undefined)
                return nextParent

            nextParent = nextParent.parent
        }
        return null
    }

    Component.onCompleted: {
        showTestNote();
    }

}
