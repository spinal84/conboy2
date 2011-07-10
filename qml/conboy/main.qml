import QtQuick 1.0
import Conboy 1.0

Rectangle {
    width: 800
    height: 480

    Flickable {
        id: flickable
        contentHeight: editor.height
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: toolbar.top

        TextEditor {
            id: editor
            width: flickable.width
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

        Rectangle {
            color: "gray"
            radius: 10
            height: 60
            width: 100
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter

            Text {
                text: "Quit"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    Qt.quit();
                }
            }
        }
    }
}
