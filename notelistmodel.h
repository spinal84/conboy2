#ifndef NOTELISTMODEL_H
#define NOTELISTMODEL_H

#include <QAbstractListModel>

#include "notedata.h"
#include "notestore.h"

/*
  A list that can be sorted and filtered. The content of this list
  will be displayed to the user through QML.

  This list can be used in a list view of all notes. For search results.
  And filtered by note book tag it can be used to display note books.

  It should be able to sort this list by modification date and title of
  the containing notes.
*/

class NoteListModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum NoteRoles {
        UuidRole = Qt::UserRole + 100,
        TitleRole,
        LastChangeDateRole
    };

    explicit NoteListModel(NoteStore *store, QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:
    void sortByDate();
    void sortByTitle();

private slots:
    void addNote(NoteData *note);

private:
    NoteStore *store;
    QList<NoteData*> notes;

};

#endif // NOTELISTMODEL_H
