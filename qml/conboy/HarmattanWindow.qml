import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    initialPage: listPage

    platformStyle: PageStackWindowStyle {
        cornersVisible: true
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
