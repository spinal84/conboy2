import QtQuick 1.0

Item {
    id: root
    property QtObject listView

    function showNote(uuid) {
        editor.showNote(uuid)
    }

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

        // Prevent clicks from going through
        MouseArea {
            anchors.fill: parent
        }

        Row {
            spacing: 20
            anchors.fill: parent
            anchors.margins: 10

            Button {
                text: "Bold"
                checked: editor.bold
                onClicked: editor.toggleBold()
            }

            Button {
                text: "Increase"
                onClicked: editor.increaseIndent()
            }

            Button {
                text: "Decrease"
                onClicked: editor.decreaseIndent()
            }

            Button {
                text: "List"
                onClicked: {
                    editor.save()
                    listView.incrementCurrentIndex()
                }
            }

            Button {
                text: "New"
                onClicked: {
                    editor.save()
                    editor.newNote()
                }
            }

            Button {
                text: "Xml"
                onClicked: editor.getXml()
            }
        }
    }
}
