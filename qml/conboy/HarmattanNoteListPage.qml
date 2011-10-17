import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    signal showEditor
    signal newNote
    property QtObject editor

    Menu {
        id: menu
        MenuLayout {
            MenuItem {text: "<s>Delete notes</s>"}
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

    TextField {
        id: search
        platformStyle: TextFieldStyle {}
        placeholderText: "Search"
        anchors { top: header.bottom; left: parent.left; right: parent.right }
        anchors.margins: 16 // TODO: Use platform margins
        inputMethodHints: Qt.ImhNoPredictiveText | Qt.ImhPreferLowercase | Qt.ImhNoAutoUppercase
        onTextChanged: noteListModel.setFilterFixedString(text)

        Image {
            anchors { top: parent.top; right: parent.right; margins: 5 }
            smooth: true
            fillMode: Image.PreserveAspectFit
            source: search.text ? "image://theme/icon-m-input-clear" : "image://theme/icon-m-common-search"
            height: parent.height - platformStyle.paddingMedium * 2
            width: parent.height - platformStyle.paddingMedium * 2

            MouseArea {
                anchors.fill: parent
                anchors.margins: -10 // Make area bigger then image
                enabled: search.text
                onClicked: search.text = ""
            }
         }
     }

    ListView {
        id: noteList
        model: noteListModel
        clip: true
        anchors { top: search.bottom; left: parent.left; right: parent.right; bottom: parent.bottom }
        anchors.margins: 16 // TODO: Use platform margins

        delegate: HarmattanNoteListDelegate {
            width: noteList.width
            uuid: model.uuid
            title: model.title
            lastChangeDate: model.lastChangeDate
            favorite: model.favorite
            onClicked: {
                editor.showNote(uuid)
                showEditor()
            }
            onPressAndHold: {
                noteList.currentIndex = index
                contextMenu.open()
            }
        }
    }

    ContextMenu {
        id: contextMenu
        MenuLayout {
            MenuItem {
                text: noteList.currentItem.favorite ? "Remove from favorites": "Add to favorites"
                //onClicked: noteList.currentItem.favorite = !noteList.currentItem.favorite
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
        message: noteList.currentItem.title
        acceptButtonText: "Yes"
        rejectButtonText: "No"
        onAccepted: noteStore.del(noteList.currentItem.uuid)
    }

    ScrollDecorator {
        flickableItem: noteList
    }
}
