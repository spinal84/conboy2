import QtQuick 1.0
import Conboy 1.0

Rectangle {
    width: 800
    height: 480
    color: "lightgreen"

    Flickable {
        id: flickable
        contentHeight: editor.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: toolbar.top

        ConboyEditor {
            id: editor
            width:  flickable.width
            // We want the editor always at least as high as the flickable
            minHeight: flickable.height
        }
    }

    Rectangle {
        id: toolbar
        color: "blue"
        height: 80
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        ToggleButton {
            id: boldBut
            text: "Bold"
            toggled: editor.bold
            onClicked: editor.toggleBold()
        }
    }
}
