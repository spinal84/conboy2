#include <QXmlSimpleReader>
#include <QDebug>
#include <QDomDocument>
#include <QXmlStreamReader>

#include "tomboystorage.h"
#include "tomboyxmlstreamreader.h"

TomboyStorage::TomboyStorage(QObject *parent) :
    QObject(parent)
{
    // TODO: Check if exists, etc.^
    pathToXmlFiles = QDir::home();
    pathToXmlFiles.cd(".conboy");
}

NoteData* TomboyStorage::load(QUuid uuid)
{
    // Remove curly brakets that souround the UUID (TODO: Maybe completely switch to strings for UUIDs)
    QString uuidStr = uuid.toString();
    uuidStr.remove(0, 1);
    uuidStr.chop(1);

    QString path = pathToXmlFiles.absoluteFilePath(uuidStr + ".note");
    QFile file(path);

    if (!file.exists()) {
        qDebug() << "ERROR: File does not exist:" << file.fileName();
        return 0;
    }

    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "ERROR: Cannot open file: " << file.fileName();
        return 0;
    }


    NoteData *note = new NoteData();
    note->setUuid(uuidStr);

    TomboyXmlStreamReader xml(&file);

    // TODO: Maybe optimize by using a hash with select/case
    // TODO: Reuse the same TomboyXmlStreamReader for all notes
    // TODO: Put the following code into TomboxXmlStreamReader class
    while (!xml.atEnd()) {
        if (xml.readNext() == QXmlStreamReader::StartElement) {

            if (xml.name() == "title") {
                note->setTitle(xml.readElementText());
                continue;
            }

            if (xml.name() == "note-content") {
                note->setContent(xml.innerXml());
                continue;
            }

            if (xml.name() == "last-change-date") {
                note->setLastChangeDate(QDateTime::fromString(xml.readElementText(), Qt::ISODate));
                continue;
            }

            if (xml.name() == "last-metadata-change-date") {
                note->setLastMetadataChangeDate(QDateTime::fromString(xml.readElementText(), Qt::ISODate));
                continue;
            }

            if (xml.name() == "create-date") {
                note->setCreateDate(QDateTime::fromString(xml.readElementText(), Qt::ISODate));
                continue;
            }
        }
    }

    if (xml.hasError()) {
        qDebug() << "Error:" << xml.errorString();
    }

    return note;
}



bool TomboyStorage::save(NoteData *note)
{

}

bool TomboyStorage::del(NoteData *note)
{

}

QList<QUuid> TomboyStorage::getAllUuids()
{
    QStringList filter;
    filter << "*.note";
    QStringList filenames = pathToXmlFiles.entryList(filter, QDir::Files);

    QList<QUuid> uuids;
    for (int i = 0; i < filenames.count(); i++) {
        QString filename = filenames[i];
        filename.chop(5); // Remove '.note'
        QUuid uuid(filename);
        uuids.append(uuid);
    }

    return uuids;
}
