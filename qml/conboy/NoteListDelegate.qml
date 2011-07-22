import QtQuick 1.0

Rectangle {
    id: root

    signal clicked(string uuid)

    property string uuid: "123-456-789"
    property string title: "Unknown title"
    property date lastChangeDate: "2000-01-01"

    width: 600
    height: 60
    color: mouse.pressed ? "orange" : "lightgreen"

    Text {
        text: title
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }

    Text {
        text: Qt.formatDateTime(lastChangeDate)
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.verticalCenter: parent.verticalCenter
    }

    Rectangle {
        color: "white"
        height: 1
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: root.clicked(uuid)
    }
}
