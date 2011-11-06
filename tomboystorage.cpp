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
    // TODO: Check if exists, etc.
    pathToXmlFiles = QDir::home();
    pathToXmlFiles.cd("MyDocs");
    if (!pathToXmlFiles.cd(".conboy")) {
        pathToXmlFiles.mkdir(".conboy");
        pathToXmlFiles.cd(".conboy");
    }
    qDebug() << "INFO: Storing notes in:" << pathToXmlFiles.absolutePath();
}

NoteData* TomboyStorage::load(QString uuid)
{
    QString path = pathToXmlFiles.absoluteFilePath(uuid + ".note");
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
    note->setUuid(uuid);

    TomboyXmlStreamReader xml(&file);

    // TODO: Maybe optimize by using a hash with select/case
    // TODO: Reuse the same TomboyXmlStreamReader for all notes
    // TODO: Put the following code into TomboyXmlStreamReader class
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

    QString path = pathToXmlFiles.absoluteFilePath(note->getUuid() + ".note");
    qDebug() << "SAVE TO: " << path;

    QFile file(path);

    if (!file.open(QFile::ReadWrite)) {
        qDebug() << "ERROR: File cannot be opened";
        return false;
    }


    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);
    writer.setAutoFormattingIndent(2);
    writer.writeStartDocument();

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

    // Write content
    // TODO: Is there a more efficient way? Now we're reparsing the xml-string just
    // to write it out. Maybe store in parsed form?
    qDebug() << "###########################";
    qDebug() << note->getContent();
    qDebug() << "###########################";

    QXmlStreamReader reader(note->getContent());
    // Add namespaces, otherwise we get parse errors
    reader.addExtraNamespaceDeclaration(QXmlStreamNamespaceDeclaration("link", "http://beatniksoftware.com/tomboy/link"));
    reader.addExtraNamespaceDeclaration(QXmlStreamNamespaceDeclaration("size", "http://beatniksoftware.com/tomboy/size"));

    while (reader.name() != "note-content") {
        reader.readNext();
    }

    while (! (reader.isEndElement() && reader.name() == "note-content")) {
        writer.writeCurrentToken(reader);
        reader.readNext();
    }
    // Write closing tag (</note-content>)
    writer.writeCurrentToken(reader);
    reader.clear();

    // Close Text
    writer.writeEndElement();

    // Meta-Data
    writer.setAutoFormatting(true);
    writer.writeTextElement("last-change-date", note->getLastChangeDate().toString(Qt::ISODate));
    writer.writeTextElement("last-metadata-change-date", note->getLastMetadataChangeDate().toString(Qt::ISODate));
    writer.writeTextElement("create-date", note->getCreateDate().toString(Qt::ISODate));
    writer.writeTextElement("cursor-position", QString::number(note->getCursorPosition()));
    writer.writeTextElement("width", QString::number(note->getWidth()));
    writer.writeTextElement("height", QString::number(note->getHeight()));
    writer.writeTextElement("x", QString::number(note->getX()));
    writer.writeTextElement("y", QString::number(note->getY()));
    writer.writeTextElement("open-on-startup", note->getOpenOnStartup() ? "True" : "False");

    writer.writeEndDocument();

    file.close();
    return true;
}

bool TomboyStorage::del(QString uuid)
{
    QString path = pathToXmlFiles.absoluteFilePath(uuid + ".note");
    QFile file(path);

    if (!file.exists()) {
        qDebug() << "ERROR: Cannot delete. File does not exist:" << file.fileName();
        return false;
    }

    if (file.remove()) {
        qDebug() << "INFO: Removed file: " << file.fileName();
        return true;
    }

    qDebug() << "ERROR: Could note delete note.";
    return false;
}

QList<QString> TomboyStorage::getAllUuids()
{
    QStringList filter;
    filter << "*.note";
    QStringList filenames = pathToXmlFiles.entryList(filter, QDir::Files);

    QList<QString> uuids;
    for (int i = 0; i < filenames.count(); i++) {
        QString filename = filenames[i];
        filename.chop(5); // Remove '.note'
        uuids.append(filename);
    }

    return uuids;
}
