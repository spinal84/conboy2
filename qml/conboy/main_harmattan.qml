import QtQuick 1.0
import Conboy 1.0
import com.nokia.meego 1.0

Window {

    StatusBar {
        id: statusBar
        anchors.top: parent.top
    }

    Flickable {
        id: flickable
        contentHeight: editor.height
        anchors.top: statusBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: toolbar.top

        TextEditor {
            id: editor
            width: flickable.width
            // We want the editor always at least as high as the flickable
            minHeight: flickable.height
            fontSize: 40
        }
    }

    ToolBar {
        id: toolbar
        anchors.bottom: parent.bottom

        tools: ToolBarLayout {
            ToolButton {
                text: "Bold"
                onClicked: editor.makeBold()
            }
        }
    }
}
