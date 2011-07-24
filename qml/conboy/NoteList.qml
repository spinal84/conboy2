import QtQuick 1.0

ListView {
    id: root

    signal clicked(string uuid)

    width: 800
    height: 480
    model: noteListModel
    delegate: NoteListDelegate {
        width: root.width
        uuid: model.uuid
        title: model.title
        lastChangeDate: model.lastChangeDate
        onClicked: root.clicked(uuid)
    }
}
