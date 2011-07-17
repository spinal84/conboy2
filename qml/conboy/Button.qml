import QtQuick 1.0

Rectangle {
    id: root
    signal clicked
    property bool checked: false
    property alias text: txt.text

    color: "gray"
    radius: 10
    height: 60
    width: 100

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
            name: "checked"
            when: root.checked
            PropertyChanges {
                target: root
                color: "lightblue"
            }
        }
    ]
}
