#include <QXmlSimpleReader>
#include <QDebug>
#include <QDomDocument>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

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
    qDebug() << "SAVING NOTE:" << note->getUuid();

    // Remove curly brakets that souround the UUID (TODO: Maybe completely switch to strings for UUIDs)
    QString uuidStr = note->getUuid().toString();
    uuidStr.remove(0, 1);
    uuidStr.chop(1);

    QString path = pathToXmlFiles.absoluteFilePath(uuidStr + ".note");
    qDebug() << "SAVE TO: " << path;

    QFile file(path);

    if (!file.open(QFile::ReadWrite)) {
        qDebug() << "ERROR: File cannot be opened";
        return false;
    }


    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);
    writer.writeStartDocument();

    /*
<note version="0.3" xmlns:link="http://beatniksoftware.com/tomboy/link" xmlns:size="http://beatniksoftware.com/tomboy/size" xmlns="http://beatniksoftware.com/tomboy">
  <title>Kino</title>
  <text xml:space="preserve"><note-content version="0.1">Kino

Sa 23.00 Internat5 Schauburg
</note-content></text>
  <last-change-date>2011-04-16T19:21:05.6790870+02:00</last-change-date>
  <last-metadata-change-date>2011-04-16T19:21:05.6790870+02:00</last-metadata-change-date>
  <create-date>2011-04-16T19:18:50.7360790+02:00</create-date>
  <cursor-position>0</cursor-position>
  <width>450</width>
  <height>360</height>
  <x>0</x>
  <y>0</y>
  <open-on-startup>False</open-on-startup>
</note>

*/

    // Note
    writer.writeStartElement("note");
    writer.writeAttribute("version", "0.3");
    writer.writeNamespace("http://beatniksoftware.com/tomboy/link", "link");
    writer.writeNamespace("http://beatniksoftware.com/tomboy/size", "size");
    writer.writeDefaultNamespace("http://beatniksoftware.com/tomboy");

    // Title
    writer.writeTextElement("title", note->getTitle());

    // Text
    writer.writeStartElement("text");
    writer.writeAttribute("xml:space", "preserve");

    // Content
    writer.setAutoFormatting(false);
    // TODO: Insert content -- Looks like there is no .writeRaw()
    writer.writeCharacters("content content <b>content</b> content conteten\ncontent contetnet");

    // Close Text
    writer.writeEndElement();

    // Meta-Data
    // TODO: Add time formatting
    writer.setAutoFormatting(true);
    writer.writeTextElement("last-change-date", note->getLastChangeDate().toString());
    writer.writeTextElement("last-metadata-change-date", note->getLastMetadataChangeDate().toString());

    writer.writeEndDocument();

    file.close();
    return true;
}

bool TomboyStorage::del(NoteData *note)
{
    return true;
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
