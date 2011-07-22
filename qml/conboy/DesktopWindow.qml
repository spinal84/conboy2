import QtQuick 1.0
import Conboy 1.0

Rectangle {
    id: root
    width: 800
    height: 480
    color: "lightgreen"

    VisualItemModel {
        id: itemModel

        DesktopNoteEditPage {
            width: root.width
            height: root.height
            listView: view
        }

        DesktopNoteListPage {
            width: root.width
            height: root.height
            listView: view
        }
    }

    ListView {
        id: view
        orientation: ListView.Horizontal
        interactive: false
        highlightMoveDuration: 400
        model: itemModel
        anchors.fill: parent
    }
}
