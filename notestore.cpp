#include <QTime>
#include <QDebug>

#include "notestore.h"
#include "tomboystorage.h"

NoteStore::NoteStore(QObject *parent) :
    QObject(parent)
{
}

void NoteStore::loadAll()
{
    QTime startTime;
    startTime.start();

    TomboyStorage storage;
    QList<QUuid> allUuids = storage.getAllUuids();

    for (int i = 0; i < allUuids.count(); i++) {
        NoteData *note = storage.load(allUuids[i]);
        notes.insert(allUuids[i], note);
    }

    qDebug() << "Parsing " << allUuids.length() << " notes took " << startTime.elapsed() << " ms.";
}

void NoteStore::addNote(QUuid uuid, NoteData* note)
{
    notes.insert(uuid, note);
}

NoteData* NoteStore::findNote(QUuid uuid)
{
    return notes.value(uuid);
}

NoteData* NoteStore::findNote(QString uuid)
{
    return notes.value(QUuid(uuid));
}

int NoteStore::count()
{
    return notes.count();
}

NoteData* NoteStore::get(int index)
{
    return notes.values()[index];
}

QList<NoteData*> NoteStore::getNotes()
{
    return notes.values();
}

