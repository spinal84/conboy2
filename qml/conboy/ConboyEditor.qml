import QtQuick 1.0
import Conboy 1.0

FocusScope {
    property alias minHeight: editor.minHeight
    property alias bold: editor.bold
    property alias fontSize: editor.fontSize

    function toggleBold() {
        editor.toggleBold()
    }

    function increaseIndent() {
        editor.increaseIndent()
    }

    function decreaseIndent() {
        editor.decreaseIndent()
    }

    // Height is defined by the editor (content)
    height: editor.height

    TextEditor {
        id: editor
        fontSize: 20
        // Width is defined by the parent
        width: parent.width

        Component.onCompleted: {
            editor.showTestNote()
            // Without this, we can't enter text
            editor.forceActiveFocus()
        }

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
    }
}
