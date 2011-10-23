#include "notedata.h"

NoteData::NoteData(QObject *parent) :
    QObject(parent)
{
    QString uuidStr = QUuid::createUuid().toString();
    uuidStr.remove(0, 1);
    uuidStr.chop(1);
    uuid = uuidStr;
    QDateTime currentTime = QDateTime::currentDateTime();
    createDate = currentTime;
    lastChangeDate = currentTime;
    lastMetadataChangeDate = currentTime;
    cursorPosition = 0;
    width = 200;
    heigth = 300;
    x = 50;
    y = 50;
    openOnStartup = false;
    favorite = false;
}

QString NoteData::getUuid() const
{
    return uuid;
}

void NoteData::setUuid(QString uuid)
{
    this->uuid = uuid;
}

QString NoteData::getTitle() const
{
    return title;
}

void NoteData::setTitle(QString title)
{
    if (title != this->title) {
        this->title = title;
        emit titleChanged();
    }
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
    if (time != lastChangeDate) {
        lastChangeDate = time;
        emit lastChangeDateChanged();
    }
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

bool NoteData::getFavorite() const
{
    return favorite;
}

void NoteData::setFavorite(bool fav)
{
    if (favorite != fav) {
        favorite = fav;
        emit favoriteChanged();
    }
}

bool NoteData::getSelected() const
{
    return selected;
}

void NoteData::setSelected(bool sel)
{
    if (selected != sel) {
        selected = sel;
        emit selectedChanged();
    }
}

void NoteData::setStore(NoteStore *store)
{
    this->store = store;
}

void NoteData::save()
{
    store->save(this);
}


