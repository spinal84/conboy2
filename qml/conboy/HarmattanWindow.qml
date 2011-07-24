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
        id: editPage
        onShowList: stack.push(listPage)
    }

    HarmattanNoteListPage {
        id: listPage
        editor: editPage
    }

    Component.onCompleted: stack.push(editPage)
}
