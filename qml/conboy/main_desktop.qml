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
                text: "Bold"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    editor.makeBold()
                }
            }
        }
    }
}
