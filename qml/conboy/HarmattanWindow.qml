import QtQuick 1.0
import com.nokia.meego 1.0

Window {

    StatusBar {
        id: statusBar
        anchors.top: parent.top
    }

    PageStack {
        id: stack
        toolBar: bar
        anchors.left: parent.left
        anchors.top:  statusBar.bottom
        anchors.right: parent.right
        anchors.bottom: toolBar.top
    }

    ToolBar {
        id: bar
        anchors.bottom: parent.bottom
    }

    HarmattanNoteEditPage {
        id: editor
        onShowList: stack.push(noteList)
    }

    HarmattanNoteListPage {
        id: noteList
    }

    Component.onCompleted: stack.push(editor)
}
