import QtQuick 1.0
import Conboy 1.0

FocusScope {
    property alias minHeight: editor.minHeight
    property alias bold: editor.bold
    property alias fontSize: editor.fontSize

    function showNote(uuid)
    {
        console.log("Open note with uuid: " + uuid)
        var note = noteStore.findNote(uuid)
        editor.showNote(note)
    }

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

        // Margins, when to start scrolling
        property int topMargin: 100
        property int bottomMargin: 100

        Component.onCompleted: {
            editor.showTestNote()
            // Without this, we can't enter text
            editor.forceActiveFocus()
        }

        // TODO: Create onCursorYPositionChanged. We don't need X-pos
        // TODO: Add topMargin and bottomMargin to trigger scrolling earlier, not just if unavoidable
        onCursorPositionChanged: {

            var flickable = findFlickable(parent)
//            console.log("TextArea height: " + flickable.height)
//            console.log("Cursor position: " + rect.y)
//            console.log("ContentY: " + flickable.contentY)

            // If cursor is below visible area or below visible area, call scrolling code
            if (((rect.y + rect.height) > (flickable.height + flickable.contentY)) ||
                    (rect.y < flickable.contentY)) {

                var newContentY = rect.y + rect.height - flickable.height

                // If overpanned, set contentY to max possible value (reached bottom)
                if (newContentY > flickable.contentHeight - flickable.height) {
                    newContentY = flickable.contentHeight - flickable.height
                }

                // If overpanned, set contentY to min possible value (reached top)
                if (newContentY < 0) {
                    newContentY = 0
                }

    //             // Cursor is below visible area
    //            if ((rect.y + rect.height) > (flickable.height + flickable.contentY)) {
    //                console.log("Out of window. BOTTOM")
    //                contentMovingAnimation.target = flickable
    //                contentMovingAnimation.to = newContentY
    //                contentMovingAnimation.running = true
    //                return
    //            }

    //             // Cursor is above visible area
    //            if (rect.y < flickable.contentY) {
    //                console.log("Out of window. TOP")
    //                contentMovingAnimation.target = flickable
    //                contentMovingAnimation.to = newContentY
    //                contentMovingAnimation.running = true
    //                return
    //            }

                // It might make sense to separate above and below. The default
                // TextArea does it like that.
                if (newContentY != flickable.contentY) {
                    contentMovingAnimation.target = flickable
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
