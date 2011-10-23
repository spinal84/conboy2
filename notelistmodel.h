#ifndef NOTELISTMODEL_H
#define NOTELISTMODEL_H

#include <QAbstractListModel>

#include "notedata.h"
#include "notestore.h"

class NoteListModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum NoteRoles {
        UuidRole = Qt::UserRole + 100,
        TitleRole,
        LastChangeDateRole,
        FavoriteRole,
        SelectedRole,
        NoteRole
    };

    explicit NoteListModel(NoteStore *store, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private slots:
    void addNote(NoteData *note);
    void removeNote(NoteData *note);
    void onNoteChanged();

private:
    NoteStore *store;
    QList<NoteData*> notes;

};

#endif // NOTELISTMODEL_H
