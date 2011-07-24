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

    ListView {
        id: noteList

        model: noteListModel

        anchors.fill: parent

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


//    NoteList {
//        id: noteList
//        anchors.fill: parent
//        delegate: HarmattanNoteListDelegate {
//            width: root.width
//            uuid: model.uuid
//            title: model.title
//            lastChangeDate: model.lastChangeDate
//            onClicked: {
//                editor.showNote(uuid)
//                pageStack.pop()
//            }
//        }
////        onClicked: {
////            editor.showNote(uuid)
////            pageStack.pop()
////        }
//    }

    ScrollDecorator {
        flickableItem: noteList
    }
}
