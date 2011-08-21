#include "notedata.h"

NoteData::NoteData(QObject *parent) :
    QObject(parent)
{
    uuid = QUuid::createUuid();
    QDateTime currentTime = QDateTime::currentDateTime();
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

int NoteData::getCursorPosition() const
{
    return cursorPosition;
}

void NoteData::setCursorPosition(int pos)
{
    cursorPosition = pos;
}

int NoteData::getWidth() const
{
    return width;
}

void NoteData::setWidth(int width)
{
    this->width = width;
}

int NoteData::getHeight() const
{
    return heigth;
}

void NoteData::setHeight(int heigth)
{
    this->heigth = heigth;
}

int NoteData::getX() const
{
    return x;
}

void NoteData::setX(int x)
{
    this->x = x;
}

int NoteData::getY() const
{
    return y;
}

void NoteData::setY(int y)
{
    this->y = y;
}

bool NoteData::getOpenOnStartup() const
{
    return openOnStartup;
}

void NoteData::setOpenOnStartup(bool open)
{
    openOnStartup = open;
}

void NoteData::setStore(NoteStore *store)
{
    this->store = store;
}

void NoteData::save()
{
    store->save(this);
}


