#include <QDebug>
#include "notelistmodel.h"

NoteListModel::NoteListModel(NoteStore *store, QObject *parent) :
    QAbstractListModel(parent), store(store)
{
    QHash<int, QByteArray> roles;
    roles[UuidRole] = "uuid";
    roles[TitleRole] = "title";
    roles[LastChangeDateRole] = "lastChangeDate";
    setRoleNames(roles);

    notes = store->getNotes();

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
    default: return QVariant();
    }
}

bool compareTitle(const NoteData *n1, const NoteData *n2)
{
    if (n1 == 0 || n2 == 0) {
        return false;
    }

    return n1->getTitle().toLower() < n2->getTitle().toLower();
}

bool compareLastChangeDate(const NoteData *n1, const NoteData *n2)
{
    if (n1 == 0 || n2 == 0) {
        return false;
    }

    return n1->getLastChangeDate() > n2->getLastChangeDate();
}

// TODO: Maybe add "Qt::SortOrder order" as parameter
void NoteListModel::sortByDate()
{
    emit layoutAboutToBeChanged();
    qStableSort(notes.begin(), notes.end(), compareLastChangeDate);
    emit layoutChanged();
}

// TODO: Maybe add "Qt::SortOrder order" as parameter
void NoteListModel::sortByTitle()
{
    emit layoutAboutToBeChanged();
    qStableSort(notes.begin(), notes.end(), compareTitle);
    emit layoutChanged();
}

void NoteListModel::addNote(NoteData *note)
{
    emit layoutAboutToBeChanged();
    notes.append(note);
    emit layoutChanged();
}
