import QtQuick 1.0
import com.nokia.meego 1.0

Item {
    id: root

    signal clicked(string uuid)

    property string uuid: "123-456-789"
    property string title: "Unknown title"
    property date lastChangeDate: "2000-01-01"

    width: 600
    height: 70

    BorderImage {
        id: background
        anchors.fill: parent
        visible: mouse.pressed
        source: "image://theme/meegotouch-list-background-pressed-center"
    }

    Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10

        Text {
            text: title
            font.pointSize: 18
            font.bold: true
        }

        Text {
            text: Qt.formatDateTime(lastChangeDate)
            font.pointSize: 12
        }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: root.clicked(uuid)
    }
}



