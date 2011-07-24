import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    id: root

    signal showList

    function showNote(uuid) {
        editor.showNote(uuid)
    }

    Flickable {
        id: flickable
        contentHeight: editor.height
        anchors.fill:  parent

        ConboyEditor {
            id: editor
            width:  flickable.width
            // We want the editor always at least as high as the flickable
            minHeight: flickable.height
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

    tools: ToolBarLayout {
        ToolButton {
            id: boldBut
            text: "Bold"
            checked: editor.bold
            onClicked: editor.toggleBold()
        }
        ToolIcon {
            iconId: "icon-m-toolbar-view-menu"
            onClicked: root.showList()
        }
    }
}
