import QtQuick 1.0

ListView {
    id: noteList
    width: 800
    height: 480
    model: noteListModel
    delegate: NoteListDelegate {
        width: noteList.width
        uuid: model.uuid
        title: model.title
        lastChangeDate: model.lastChangeDate
        onClicked: console.log("Open note with uuid: " + uuid)
    }
}
