#ifndef NOTESTORE_H
#define NOTESTORE_H

#include <QHash>
#include <QUuid>

#include "notedata.h"

class NoteData;


/***
  NoteStore knows about all notes, no matter where
  they are coming from.
  ***/
class NoteStore : public QObject
{
    Q_OBJECT

public:
    NoteStore(QObject *parent = 0);
    void addNote(QUuid uuid, NoteData* note);
    int count();
    NoteData* get(int index);
    QList<NoteData*> getNotes();

public slots:
    NoteData* newNote();
    NoteData* findNote(QUuid uuid);
    NoteData* findNote(QString uuid);
    void loadAll();
    void save(NoteData *note);

signals:
    void noteAdded(NoteData *note);
    void noteRemoved(NoteData *note);

private:
    QHash<QUuid, NoteData*> notes;
};

#endif // NOTESTORE_H
