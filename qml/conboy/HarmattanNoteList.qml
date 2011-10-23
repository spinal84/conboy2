import QtQuick 1.0
import com.nokia.meego 1.0

Item {
    property bool multiSelect: false
    property alias currentItem: noteList.currentItem

    TextField {
        id: search
        platformStyle: TextFieldStyle {}
        placeholderText: "Search"
        anchors { top: parent.top; left: parent.left; right: parent.right }
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
            selected: model.selected
            onClicked: {
                if (multiSelect) {
                    noteList.currentIndex = index
                    noteStore.toggleSelected(noteList.currentItem.uuid)
                } else {
                    editor.showNote(uuid)
                    showEditor()
                }
            }
            onPressAndHold: {
                if (!multiSelect) {
                    noteList.currentIndex = index
                    contextMenu.open()
                }
            }
        }
    }

    ScrollDecorator {
        flickableItem: noteList
    }
}
