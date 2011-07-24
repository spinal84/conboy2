import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    initialPage: editPage

    HarmattanNoteEditPage {
        id: editPage
        onShowList: pageStack.push(listPage)
    }

    HarmattanNoteListPage {
        id: listPage
        editor: editPage
    }
}
