#include <QDebug>
#include "notelistmodel.h"

NoteListModel::NoteListModel(NoteStore *store, QObject *parent) :
    QAbstractListModel(parent), store(store)
{
    QHash<int, QByteArray> roles;
    roles[UuidRole] = "uuid";
    roles[TitleRole] = "title";
    roles[LastChangeDateRole] = "lastChangeDate";
    roles[FavoriteRole] = "favorite";
    setRoleNames(roles);

    notes = store->getNotes();

    // Connect handlers to all notes that are already in the store
    for (int i = 0; i < store->count(); i++) {
        connect(notes[i], SIGNAL(titleChanged()), this, SLOT(onNoteChanged()));
        connect(notes[i], SIGNAL(lastChangeDateChanged()), this, SLOT(onNoteChanged()));
        connect(notes[i], SIGNAL(favoriteChanged()), this, SLOT(onNoteChanged()));
    }

    // All notes that are added afterwards will trigger the addNote slot
    connect(store, SIGNAL(noteAdded(NoteData*)), this, SLOT(addNote(NoteData*)));
}

int NoteListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return notes.count();
}

QVariant NoteListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > notes.count()) {
        return QVariant();
    }

    NoteData *note = notes[index.row()];

    switch (role) {
    case UuidRole: return note->getUuid().toString();
    case TitleRole: return note->getTitle();
    case LastChangeDateRole: return note->getLastChangeDate();
    case FavoriteRole: return note->getFavorite();
    default: return QVariant();
    }
}

// TODO: On startup addNote() is called often. With each call the list gets sorted.
// Optimize that.
void NoteListModel::addNote(NoteData *note)
{
    emit layoutAboutToBeChanged();
    notes.append(note);

    // Update list if note changed
    qDebug() << "Connect signal to note: " << note->getTitle();
    connect(note, SIGNAL(titleChanged()), this, SLOT(onNoteChanged()));
    connect(note, SIGNAL(lastChangeDateChanged()), this, SLOT(onNoteChanged()));
    connect(note, SIGNAL(favoriteChanged()), this, SLOT(onNoteChanged()));
    emit layoutChanged();
}

void NoteListModel::onNoteChanged()
{
    qDebug() << "INFO: Note changed";
    // If a Note has changed, find out which and update that one
    NoteData *note = (NoteData*)sender();
    int i = notes.indexOf(note);
    emit dataChanged(index(i), index(i));
}
