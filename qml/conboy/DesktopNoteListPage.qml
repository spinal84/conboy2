import QtQuick 1.0

Item {
    property QtObject listView

    width: 800
    height: 480

    NoteList {
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.bottom: toolbar.top
    }

    Rectangle {
        id: toolbar
        color: "blue"
        height: 80
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        Row {
            spacing: 20
            anchors.fill: parent
            anchors.margins: 10

            Button {
                text: "Back"
                onClicked: listView.decrementCurrentIndex()
            }
        }
    }
}
