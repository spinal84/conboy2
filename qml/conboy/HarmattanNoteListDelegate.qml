import QtQuick 1.0
import com.nokia.meego 1.0

Item {
    id: root

    signal clicked(string uuid)
    signal pressAndHold(string uuid)

    property string uuid: "123-456-789"
    property string title: "Unknown title"
    property date lastChangeDate: "2000-01-01"
    property bool favorite: false
    property bool selected: false

    width: 600
    height: 70

    BorderImage {
        id: selectedBackground
        anchors.fill: parent
        visible: selected
        source: "image://theme/color6-meegotouch-list-background-selected-center"
    }

    BorderImage {
        id: background
        anchors.fill: parent
        visible: mouse.pressed
        source: "image://theme/meegotouch-list-background-pressed-center"
    }

    Image {
        id: star
        source: "image://theme/icon-s-common-favorite-mark"
        visible: favorite
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
    }

    Column {
        anchors.left: parent.left
        anchors.right: star.left
        anchors.verticalCenter: parent.verticalCenter
        anchors.leftMargin: 10

        Text {
            text: title
            font.family: "Nokia Pure Text"
            font.pixelSize: 30
            font.bold: true
        }

        Text {
            text: Qt.formatDateTime(lastChangeDate)
            font.family: "Nokia Pure Text"
            font.pixelSize: 20
            color: "gray"
        }
    }

    MouseArea {
        id: mouse
        anchors.fill: parent
        onClicked: root.clicked(uuid)
        onPressAndHold: root.pressAndHold(uuid)
    }
}



