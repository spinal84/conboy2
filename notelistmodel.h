#ifndef NOTELISTMODEL_H
#define NOTELISTMODEL_H

#include <QAbstractListModel>

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
    explicit NoteListModel(QObject *parent = 0);



signals:

public slots:

};

#endif // NOTELISTMODEL_H
