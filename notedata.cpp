#include "notedata.h"

NoteData::NoteData(QObject *parent) :
    QObject(parent)
{
    uuid = QUuid::createUuid();
    QDateTime currentTime;
    createDate = currentTime;
    lastChangeDate = currentTime;
    lastMetadataChangeDate = currentTime;
    title = "New Note";
}

QUuid NoteData::getUuid() const
{
    return uuid;
}

void NoteData::setUuid(QUuid uuid)
{
    this->uuid = uuid;
}

QString NoteData::getTitle() const
{
    return title;
}

void NoteData::setTitle(QString title)
{
    this->title = title;
}

QString NoteData::getContent() const
{
    return content;
}

void NoteData::setContent(QString content)
{
    this->content = content;
}

QDateTime NoteData::getLastChangeDate() const
{
    return lastChangeDate;
}

void NoteData::setLastChangeDate(QDateTime time)
{
    lastChangeDate = time;
}

QDateTime NoteData::getLastMetadataChangeDate() const
{
    return lastMetadataChangeDate;
}

void NoteData::setLastMetadataChangeDate(QDateTime time)
{
    lastMetadataChangeDate = time;
}

QDateTime NoteData::getCreateDate() const
{
    return createDate;
}

void NoteData::setCreateDate(QDateTime time)
{
    createDate = time;
}
