import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    property QtObject editor

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "icon-m-toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    TextField {
        id: search
        placeholderText: "Search"
        anchors { top: parent.top; left: parent.left; right: parent.right }
        anchors.margins: 10 // TODO: Use platform margins
        Image {
            anchors { top: parent.top; right: parent.right; margins: 5 }
            smooth: true
            fillMode: Image.PreserveAspectFit
            source: "image://theme/icon-m-toolbar-search"
            height: parent.height - platformStyle.paddingMedium * 2
            width: parent.height - platformStyle.paddingMedium * 2
         }
     }

    ListView {
        id: noteList
        model: noteListModel
        clip: true
        anchors { top: search.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
        anchors.margins: 10 // TODO: Use platform margins

        delegate: HarmattanNoteListDelegate {
            width: noteList.width
            uuid: model.uuid
            title: model.title
            lastChangeDate: model.lastChangeDate
            onClicked: {
                editor.showNote(uuid)
                pageStack.pop()
            }
        }
    }

    ScrollDecorator {
        flickableItem: noteList
    }
}
