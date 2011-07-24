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
            id: editPage
            width: root.width
            height: root.height
            listView: view
        }

        DesktopNoteListPage {
            id: listPage
            width: root.width
            height: root.height
            listView: view
            editor: editPage
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
