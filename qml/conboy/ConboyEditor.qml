import QtQuick 1.0
import Conboy 1.0
import com.nokia.meego 1.0
import "TextAreaHelper.js" as TextAreaHelper

FocusScope {
    id: root

    property alias minHeight: editor.minHeight
    property alias bold: editor.bold

    property Item platformStyle

    function showNote(uuid) {
        console.log("Open note with uuid: " + uuid)
        var note = noteStore.findNote(uuid)
        editor.showNote(note)
        editor.forceActiveFocus()
    }

    function newNote() {
        console.log("New note")
        var note = noteStore.newNote()
        editor.showNote(note)
        editor.forceActiveFocus()
    }

    function getXml() {
        editor.getXml()
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

    function save() {
        editor.save()
    }

    // Height is defined by the editor (content)
    height: editor.height

    TextEditor {
        id: editor
        // Width is defined by the parent
        width: parent.width

        font.family: "Nokia Pure Text" //root.platformStyle.textFont
        font.pixelSize: 25
        color: root.platformStyle.textColor
        selectedTextColor: root.platformStyle.selectedTextColor
        selectionColor: root.platformStyle.selectionColor

        // Margins, when to start scrolling
        property int margin: 50

        Component.onCompleted: {
            editor.showTestNote()
            // Without this, we can't enter text
            editor.forceActiveFocus()
        }

        onCursorPositionChanged: {
            TextAreaHelper.repositionFlickable(contentMovingAnimation)
        }

        onTextChanged: {
            TextAreaHelper.repositionFlickable(contentMovingAnimation)
        }

        SequentialAnimation {
            id: contentMovingAnimation
            property alias target: realAnimation.target
            property alias to: realAnimation.to

            // We have to ignore mouse move events here. Otherwise the cursor and the selection
            // changes from where the move starts, to where it ends.
            ScriptAction {
                script: editor.ignoreNextMouseMoves()
            }

            PropertyAnimation {
                id: realAnimation
                property: "contentY"
                duration: 200
                easing.type: Easing.InOutCubic
            }
        }
    }

    Connections {
        target: inputContext

        onSoftwareInputPanelVisibleChanged: {
            if (activeFocus)
                TextAreaHelper.repositionFlickable(contentMovingAnimation);
        }

        onSoftwareInputPanelRectChanged: {
            if (activeFocus)
                TextAreaHelper.repositionFlickable(contentMovingAnimation);
        }
    }
}
