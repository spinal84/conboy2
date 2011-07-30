import QtQuick 1.0
import com.nokia.meego 1.0

Page {
    signal showEditor
    property QtObject editor

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "icon-m-toolbar-add"
            onClicked: console.log("TODO: Open new note")
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
