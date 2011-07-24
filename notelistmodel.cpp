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
}

int NoteListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return store->count();
}

QVariant NoteListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() > store->count()) {
        return QVariant();
    }

    NoteData *note = store->get(index.row());

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

    return n1->getTitle() < n2->getTitle();
}

bool compareLastChangeDate(const NoteData *n1, const NoteData *n2)
{
    if (n1 == 0 || n2 == 0) {
        return false;
    }

    return n1->getLastChangeDate() < n2->getLastChangeDate();
}

// column=0 -> sort by title. column=1 -> sort by last change date
void NoteListModel::sort(int column, Qt::SortOrder order)
{
    Q_UNUSED(order);
    QList<NoteData*> notes = store->getNotes();
    emit layoutAboutToBeChanged();
    if (column == 0) {
        qStableSort(notes.begin(), notes.end(), compareTitle);
    } else if (column == 1) {
        qStableSort(notes.begin(), notes.end(), compareLastChangeDate);
    }
    emit layoutChanged();
}
