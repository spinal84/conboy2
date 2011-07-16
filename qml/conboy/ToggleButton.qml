import QtQuick 1.0

Rectangle {
    id: root
    signal clicked
    property bool toggled: false
    property alias text: txt.text

    color: "gray"
    radius: 10
    height: 60
    width: 100
    anchors.left: parent.left
    anchors.leftMargin: 20
    anchors.verticalCenter: parent.verticalCenter

    Text {
        id: txt
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        onClicked: root.clicked()
    }

    states: [
        State {
            name: "toggled"
            when: root.toggled
            PropertyChanges {
                target: root
                color: "lightblue"
            }
        }
    ]
}
