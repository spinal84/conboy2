import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    id: root

    function showNote(uuid) {
        editor.showNote(uuid)
    }

    function newNote() {
        editor.newNote()
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
            platformStyle: TextAreaStyle {}
        }
    }

    ScrollDecorator {
        flickableItem: flickable
    }

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "icon-m-toolbar-back"
            onClicked: {
                editor.save()
                pageStack.pop()
            }
        }

        ToolButton {
            id: boldBut
            text: "Bold"
            checked: editor.bold
            onClicked: editor.toggleBold()
        }
    }
}
