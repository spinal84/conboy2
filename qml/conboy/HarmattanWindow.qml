import QtQuick 1.1
import com.nokia.meego 1.0

PageStackWindow {
    initialPage: editPage

    platformStyle: PageStackWindowStyle {
        cornersVisible: true
    }

    HarmattanNoteEditPage {
        id: editPage
        onShowList: pageStack.push(listPage)
    }

    HarmattanNoteListPage {
        id: listPage
        editor: editPage
    }
}
