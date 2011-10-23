#ifndef NOTEDATA_H
#define NOTEDATA_H

#include <QObject>
#include <QDateTime>
#include <QUuid>

#include "notestore.h"

class NoteStore;

/*
  This class contains all data that makes up a note.
  Not all data is important to us. E.g. we won't use window
  heigth and width on a mobile device. Still we need to save
  it in order to correctly write it back later.

  This should only be the data. Loading, saving, etc. happens
  outside this class.
*/
class NoteData : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString title READ getTitle NOTIFY titleChanged)
    Q_PROPERTY(QString content READ getContent NOTIFY contentChanged)
    Q_PROPERTY(QDateTime lastChangeDate READ getLastChangeDate NOTIFY lastChangeDateChanged)
    Q_PROPERTY(QDateTime lastMetadataChangeDate READ getLastMetadataChangeDate NOTIFY lastMetadataChangeDateChanged)
    Q_PROPERTY(QDateTime createDate READ getCreateDate NOTIFY createDateChanged)
    Q_PROPERTY(bool favorite READ getFavorite NOTIFY favoriteChanged)
    Q_PROPERTY(bool selected READ getSelected NOTIFY selectedChanged)

public:
    explicit NoteData(QObject *parent = 0);

    QString getUuid() const;
    void setUuid(QString uuid);

    QString getTitle() const;
    void setTitle(QString title);

    QString getContent() const;
    void setContent(QString content);

    QDateTime getLastChangeDate() const;
    void setLastChangeDate(QDateTime time);

    QDateTime getLastMetadataChangeDate() const;
    void setLastMetadataChangeDate(QDateTime time);

    QDateTime getCreateDate() const;
    void setCreateDate(QDateTime time);

    int getCursorPosition() const;
    void setCursorPosition(int pos);

    int getWidth() const;
    void setWidth(int width);

    int getHeight() const;
    void setHeight(int heigth);

    int getX() const;
    void setX(int x);

    int getY() const;
    void setY(int y);

    bool getOpenOnStartup() const;
    void setOpenOnStartup(bool open);

    bool getFavorite() const;
    void setFavorite(bool fav);

    bool getSelected() const;
    void setSelected(bool sel);

    void setStore(NoteStore* store);
    void save();

signals:
    void uuidChanged();
    void titleChanged();
    void contentChanged();
    void lastChangeDateChanged();
    void lastMetadataChangeDateChanged();
    void createDateChanged();
    void favoriteChanged();
    void selectedChanged();

private:
    QString uuid;
    QString title;
    QString content;
    QDateTime lastChangeDate;
    QDateTime lastMetadataChangeDate;
    QDateTime createDate;
    int cursorPosition;
    int width;
    int heigth;
    int x;
    int y;
    bool openOnStartup;
    bool favorite;
    bool selected;
    QList<QString> tags;
    NoteStore* store;

};

#endif // NOTEDATA_H
