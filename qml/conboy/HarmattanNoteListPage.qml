import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    tools: ToolBarLayout {
        ToolIcon {
            iconId: "icon-m-toolbar-back"
            onClicked: pageStack.pop()
        }
    }

    NoteList {
        anchors.fill: parent
    }

}
