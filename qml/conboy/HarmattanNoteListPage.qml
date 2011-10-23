import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    signal showEditor
    signal newNote
    property QtObject editor

    Sheet {
        id: sheet
        acceptButtonText: "Delete"
        rejectButtonText: "Cancle"
        content: HarmattanNoteList {
            multiSelect: true
            anchors.fill: parent
        }
    }

    Menu {
        id: menu
        MenuLayout {
            MenuItem {
                text: "Delete notes"
                onClicked: sheet.open()
            }
            MenuItem {text: "<s>Settings</s>"}
            MenuItem {text: "<s>Synchronization</s>"}
            MenuItem {
                text: "Sort by title"
                onClicked: noteListModel.sortByTitle()
            }
            MenuItem {
                text: "Sort by date"
                onClicked: noteListModel.sortByDate()
            }
            MenuItem {
                text: "About"
                onClicked: aboutDialog.open()
            }
        }
    }

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "icon-m-toolbar-add"
            onClicked: newNote()
        }
        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: (menu.status == DialogStatus.Closed) ? menu.open() : menu.close()
        }
    }

    Rectangle {
        id: header
        height: 72
        color: "green"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right

        Text {
            text: "Conboy"
            color: "white"
            font.family: "Nokia Pure Text"
            font.pixelSize: 32
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.verticalCenter: parent.verticalCenter
        }
    }

    HarmattanNoteList {
        id: noteList
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
    }

    ContextMenu {
        id: contextMenu
        MenuLayout {
            MenuItem {
                text: noteList.currentItem ? (noteList.currentItem.favorite ? "Remove from favorites": "Add to favorites") : ""
                onClicked: noteStore.toggleFavorite(noteList.currentItem.uuid)
            }
            MenuItem {
                text: "<s>Share note</s>"
                onClicked: console.log("TODO: implement")
            }
            MenuItem {
                text: "Delete note"
                onClicked: deleteDialog.open()
            }
        }
    }

    QueryDialog {
        id: deleteDialog
        titleText: "Delete note?"
        message: noteList.currentItem ? noteList.currentItem.title : ""
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: noteStore.del(noteList.currentItem.uuid)
    }

    QueryDialog {
        id: aboutDialog
        titleText: "Conboy"
        message: "Conboy v0.8\n(c) 2011 Cornelius Hald\nhald@icandy.de"
        acceptButtonText: "Website"
        rejectButtonText: "Close"
        onAccepted: Qt.openUrlExternally("http://zwong.de")
    }
}
