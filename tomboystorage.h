#ifndef TOMBOYSTORAGE_H
#define TOMBOYSTORAGE_H

#include <QObject>
#include <QUuid>
#include <QDir>

#include "notedata.h"

/*
  Class that can read and write the Tomboy XML format.
  Later other classes might be added. E.g. we could also save to
  sqlite or Tracker.
 */

class TomboyStorage : public QObject
{
    Q_OBJECT

public:
    explicit TomboyStorage(QObject *parent = 0);

    NoteData* load(QString uuid);
    bool save(NoteData *note);
    bool del(NoteData *note);
    QList<QString> getAllUuids();

signals:

public slots:

private:
    QDir pathToXmlFiles;

};

#endif // TOMBOYSTORAGE_H
