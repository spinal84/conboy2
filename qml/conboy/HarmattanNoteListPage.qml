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

    NoteList {
        id: noteList
        anchors.fill: parent
        onClicked: {
            editor.showNote(uuid)
            pageStack.pop()
        }
    }

    ScrollDecorator {
        flickableItem: noteList
    }
}
