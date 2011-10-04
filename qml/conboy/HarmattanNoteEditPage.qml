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

    Menu {
        id: menu
        visualParent: pageStack
        MenuLayout {
            MenuItem {text: "Delete note"; onClicked: { colorRect.color = "darkred" } }
            MenuItem {text: "Share note"; onClicked: { colorRect.color = "darkgreen" }}
        }
    }

    Menu {
        id: styleMenu
        visualParent: pageStack
        MenuLayout {
            MenuItem {text: "<b>bold</b>"}
            MenuItem {text: "<i>italic</i>"}
            MenuItem {text: "<u>underline</u>"}
            MenuItem {text: "highlight"}
            MenuItem {text: "small"}
            MenuItem {text: "normal"}
            MenuItem {text: "large"}
            MenuItem {text: "huhge"}
        }
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

        /*
          Interesting icons
          icon-m-toolbar-favorite-mark
        icon-m-toolbar-favorite-unmark
        icon-m-toolbar-edit
        icon-m-toolbar-italic
        icon-m-toolbar-list
        icon-m-toolbar-rich-text-view-menu
        icon-m-toolbar-rich-text
        icon-m-toolbar-search
        icon-m-toolbar-select-text
        icon-m-toolbar-share
        icon-m-toolbar-underline
          */

        ToolIcon {
            iconId: "icon-m-toolbar-previous"
            onClicked: (menu.status == DialogStatus.Closed) ? menu.open() : menu.close()
        }

        ToolIcon {
            iconId: "icon-m-toolbar-next"
            onClicked: (menu.status == DialogStatus.Closed) ? menu.open() : menu.close()
        }

        ToolIcon {
            iconId: "icon-m-toolbar-rich-text"
            onClicked: (styleMenu.status == DialogStatus.Closed) ? styleMenu.open() : styleMenu.close()
        }

        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: (menu.status == DialogStatus.Closed) ? menu.open() : menu.close()
        }

        /*
        ToolButton {
            id: boldBut
            text: "Bold"
            checked: editor.bold
            onClicked: editor.toggleBold()
        }
        */
    }
}
