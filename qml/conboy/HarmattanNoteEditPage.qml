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
            MenuItem {
                text: "Delete note"
                onClicked: {
                    pageStack.pop()
                    editor.deleteNote()
                }
            }
            MenuItem {
                text: "<s>Share note</s>"
                onClicked: editor.share()
            }
        }
    }

    Menu {
        id: styleMenu
        visualParent: pageStack

        Column {
            anchors.left: parent.left
            anchors.right: parent.right

            ButtonRow {
                id: fontSizeRow

                Button {
                    id: b0
                    text: "Small"
                    onClicked: editor.setFontSize(0)
                }
                Button {
                    id: b1
                    text: "Normal"
                    onClicked: editor.setFontSize(1)
                }
                Button {
                    id: b2
                    text: "Large"
                    onClicked: editor.setFontSize(2)
                }
                Button {
                    id: b3
                    text: "Huge"
                    onClicked: editor.setFontSize(3)
                }
                Binding {
                    property: "checkedButton"
                    target: fontSizeRow
                    value: {
                        switch (editor.fontSize) {
                               case 0: return b0
                               case 1: return b1
                               case 2: return b2
                               case 3: return b3
                        }
                    }
                }
            }

            /* // Could be an option to reduce required space. But needs new icons.
            ButtonRow {
                exclusive: false
                Button {
                    iconSource: "image://theme/icon-m-toolbar-bold"
                }
                Button {
                    iconSource: "image://theme/icon-m-toolbar-italic"
                }
                Button {
                    iconSource: "image://theme/icon-m-toolbar-underline"
                }
                Button {
                    iconSource: "image://theme/icon-m-toolbar-bold"
                }
                Button {
                    iconSource: "image://theme/icon-m-toolbar-bold"
                }
                Button {
                    iconSource: "image://theme/icon-m-toolbar-bold"
                }
            }
            */

            Button {
                text: "<b>Bold</b>"
                checked: editor.bold
                anchors.left: parent.left
                anchors.right: parent.right
                onClicked: editor.toggleBold()
            }
            Button {
                text: "<i>Italic</i>"
                checked: editor.italic
                anchors.left: parent.left
                anchors.right: parent.right
                onClicked: editor.toggleItalic()
            }
            Button {
                text: "<u>Underline</u>"
                checked: editor.underline
                anchors.left: parent.left
                anchors.right: parent.right
                onClicked: editor.toggleUnderline()
            }
            Button {
                text: "<s>Strikeout</s>"
                checked: editor.strikeout
                anchors.left: parent.left
                anchors.right: parent.right
                onClicked: editor.toggleStrikeout()
            }
            Button {
                text: "<span style='background-color: yellow'>Highlight</span>"
                checked: editor.highlight
                anchors.left: parent.left
                anchors.right: parent.right
                onClicked: editor.toggleHighlight()
            }
            Button {
                text: "<pre>Fixed width</pre>"
                checked: editor.fixedWidth
                anchors.left: parent.left
                anchors.right: parent.right
                onClicked: editor.toggleFixedWidth()
            }
        }
    }

    ToolBar {
        id: topBar
        anchors.top: parent.top
        tools: ToolBarLayout {
            ToolIcon {
               iconId: "icon-m-toolbar-tab-previous"
               onClicked: editor.decreaseIndent()
           }
           ToolIcon {
               iconId: "icon-m-toolbar-tab-next"
               onClicked: editor.increaseIndent()
           }
           // TODO: Add icon for 'link'
           ToolIcon {
               //iconId: "icon-m-toolbar-bold"
           }
           ToolIcon {
               iconId: "icon-m-toolbar-rich-text"
               onClicked: (styleMenu.status == DialogStatus.Closed) ? styleMenu.open() : styleMenu.close()
           }
           ToolIcon {
               iconId: "icon-m-toolbar-select-text"
           }
        }
    }

    Flickable {
        id: flickable
        contentHeight: editor.height
        clip: true
        anchors.top: topBar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom

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
            iconId: editor.favorite ? "icon-m-toolbar-favorite-mark" : "icon-m-toolbar-favorite-unmark"
            onClicked: editor.favorite = !editor.favorite
        }

        /*
        ToolIcon {
            iconId: "icon-m-toolbar-previous"
            onClicked: editor.decreaseIndent()
        }

        ToolIcon {
            iconId: "icon-m-toolbar-next"
            onClicked: editor.increaseIndent()
        }

        ToolIcon {
            iconId: "icon-m-toolbar-rich-text"
            onClicked: (styleMenu.status == DialogStatus.Closed) ? styleMenu.open() : styleMenu.close()
        }

        */
        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: (menu.status == DialogStatus.Closed) ? menu.open() : menu.close()
        }
    }
}
