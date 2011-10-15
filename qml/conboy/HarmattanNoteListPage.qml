import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    signal showEditor
    signal newNote
    property QtObject editor

    Timer {
        id: loadTimer
        repeat: false
        interval: 50
        onTriggered: noteStore.loadAll()
    }

    Component.onCompleted: {
        // Load all notes (before that wait 50ms to make sure the UI is painted)
        loadTimer.start()
    }

    Menu {
        id: menu
        visualParent: pageStack
        MenuLayout {
            MenuItem {text: "Delete notes"; onClicked: { colorRect.color = "darkred" } }
            MenuItem {text: "Settings"; onClicked: { colorRect.color = "darkgreen" }}
            MenuItem {text: "Synchronization"; onClicked: { colorRect.color = "darkblue" }}
            MenuItem {text: "Sort by title"}
            MenuItem {text: "Sort by date"}
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
        Image {
            anchors { top: parent.top; right: parent.right; margins: 5 }
            smooth: true
            fillMode: Image.PreserveAspectFit
            source: "image://theme/icon-m-toolbar-search"
            height: parent.height - platformStyle.paddingMedium * 2
            width: parent.height - platformStyle.paddingMedium * 2
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
        }
    }

    ScrollDecorator {
        flickableItem: noteList
    }
}
