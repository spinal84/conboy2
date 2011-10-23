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
    QList<QString> allUuids = storage.getAllUuids();

    for (int i = 0; i < allUuids.count(); i++) {
        NoteData *note = storage.load(allUuids[i]);
        addNote(allUuids[i], note);
    }

    qDebug() << "Parsing " << allUuids.length() << " notes took " << startTime.elapsed() << " ms.";
}

void NoteStore::save(NoteData *note)
{
    TomboyStorage storage;
    storage.save(note);
}

void NoteStore::addNote(QString uuid, NoteData* note)
{
    note->setStore(this);
    notes.insert(uuid, note);
    emit noteAdded(note);
}

QString NoteStore::newNote()
{
    qDebug() << "Creating new note";
    NoteData *note = new NoteData();
    QString num;

    num.setNum(count()+1);
    note->setTitle("New note " + num);

    QString content = "<note-content version=\"0.1\">" + note->getTitle() + "\n\n"
            + "Insert content here.\n"
            + "</note-content>";

    note->setContent(content);

    addNote(note->getUuid(), note);
    return note->getUuid();
}

NoteData* NoteStore::findNote(QString uuid)
{
    return notes.value(uuid);
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

void NoteStore::del(QString uuid)
{
    NoteData *note = findNote(uuid);
    if (note) {
        del(note);
    }
}

void NoteStore::del(NoteData *note)
{
    TomboyStorage storage;
    if (storage.del(note->getUuid())) {
        emit noteRemoved(note);
        qDebug() << "Delete NoteData object";
        notes.remove(note->getUuid());
        delete note;
    }
}

// TODO: Move into NoteListModel (implement setData() there)
void NoteStore::toggleFavorite(QString uuid)
{
    NoteData *note = findNote(uuid);
    note->setFavorite(!note->getFavorite());
}

// TODO: Move into NoteListModel (implement setData() there)
void NoteStore::toggleSelected(QString uuid)
{
    NoteData *note = findNote(uuid);
    note->setSelected(!note->getSelected());
}

void NoteStore::deleteSelected()
{
    QList<NoteData*> noteList = notes.values();
    for (int i = 0; i < noteList.length(); i++) {
        NoteData *note = noteList[i];
        if (note->getSelected()) {
            del(noteList[i]);
        }
    }
}

// TODO: Move into NoteListModel (implement setData() there)
void NoteStore::unselectAll()
{
    QList<NoteData*> noteList = notes.values();
    for (int i = 0; i < noteList.length(); i++) {
        noteList[i]->setSelected(false);
    }
}

// TODO: Move into NoteListModel (implement setData() there)
void NoteStore::selectAll()
{
    QList<NoteData*> noteList = notes.values();
    for (int i = 0; i < noteList.length(); i++) {
        noteList[i]->setSelected(true);
    }
}

// TODO: Move into NoteListModel
int NoteStore::getSelectedCount()
{
    int count = 0;
    QList<NoteData*> noteList = notes.values();
    for (int i = 0; i < noteList.length(); i++) {
        NoteData *note = noteList[i];
        if (note->getSelected()) {
            count++;
        }
    }
    return count;
}
