import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    initialPage: listPage

    platformStyle: PageStackWindowStyle {
        cornersVisible: true
    }

    Timer {
        id: loadTimer
        repeat: false
        interval: 50
        onTriggered: noteStore.loadAll()
    }

    Component.onCompleted: {
        // Load all notes (before that wait 50ms to make sure the UI is painted)
        loadTimer.start()
    }

    HarmattanNoteEditPage {
        id: editPage
    }

    HarmattanNoteListPage {
        id: listPage
        editor: editPage
        onShowEditor: pageStack.push(editPage)
        onNewNote: {
            editPage.newNote()
            pageStack.push(editPage)
        }
    }
}
