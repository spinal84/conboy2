import QtQuick 1.0
import Conboy 1.0
import com.nokia.meego 1.0
import "TextAreaHelper.js" as TextAreaHelper

FocusScope {
    id: root

    property alias minHeight: editor.minHeight
    property alias favorite: editor.favorite
    property alias bold: editor.bold
    property alias italic: editor.italic
    property alias underline: editor.underline
    property alias strikeout: editor.strikeout
    property alias highlight: editor.highlight
    property alias fixedWidth: editor.fixedWidth
    property alias fontSize: editor.fontSize
    property alias uuid: editor.uuid

    property Item platformStyle

    function showNote(uuid) {
        console.log("Open note with uuid: " + uuid)
        editor.showNote(uuid)
        editor.forceActiveFocus()
    }

    function newNote() {
        console.log("New note")
        var uuid = noteStore.newNote()
        editor.showNote(uuid)
        editor.forceActiveFocus()
    }

    function deleteNote() {
        editor.deleteNote()
    }

    function getXml() {
        editor.getXml()
    }

    function toggleBold() {
        editor.toggleBold()
    }

    function toggleItalic() {
        editor.toggleItalic()
    }

    function toggleUnderline() {
        editor.toggleUnderline()
    }

    function toggleStrikeout() {
        editor.toggleStrikeout()
    }

    function toggleHighlight() {
        editor.toggleHighlight()
    }

    function toggleFixedWidth() {
        editor.toggleFixedWidth()
    }

    function setFontSize(size) {
        editor.setFontSize(size)
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

    function share() {
        editor.share()
    }

    // Height is defined by the editor (content)
    height: editor.height

    TextEditor {
        id: editor
        // Width is defined by the parent
        width: parent.width
        store: noteStore
        font.family: "Nokia Pure Text" //root.platformStyle.textFont
        font.pixelSize: 25
        color: root.platformStyle.textColor
        selectedTextColor: root.platformStyle.selectedTextColor
        selectionColor: root.platformStyle.selectionColor

        // Margins, when to start scrolling
        property int margin: 50

        Component.onCompleted: {
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
