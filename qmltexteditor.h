#ifndef QMLTEXTEDITOR_H
#define QMLTEXTEDITOR_H

#include <QDeclarativeItem>
#include <QGraphicsProxyWidget>
#include <QTextEdit>
#include <QColor>
#include <QTimer>

#include "texteditor.h"
#include "notedata.h"
#include "notestore.h"

class QMLTextEditor : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(NoteStore* store READ getNoteStore WRITE setNoteStore NOTIFY noteStoreChanged)
    Q_PROPERTY(int minHeight READ getMinHeight WRITE setMinHeight NOTIFY minHeightChanged)
    Q_PROPERTY(bool bold READ getBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ getItalic NOTIFY italicChanged)
    Q_PROPERTY(bool underline READ getUnderline NOTIFY underlineChanged)
    Q_PROPERTY(bool strikeout READ getStrikeout NOTIFY strikeoutChanged)
    Q_PROPERTY(bool highlight READ getHighlight NOTIFY highlightChanged)
    Q_PROPERTY(bool fixedWidth READ getFixedWidth NOTIFY fixedWidthChanged)
    Q_PROPERTY(int fontSize READ getFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(QFont font READ getFont WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor selectedTextColor READ getSelectedTextColor WRITE setSelectedTextColor NOTIFY selectedTextColorChanged)
    Q_PROPERTY(QColor selectionColor READ getSelectionColor WRITE setSelectionColor NOTIFY selectionColorChanged)
    Q_PROPERTY(bool favorite READ getFavorite WRITE setFavorite NOTIFY favoriteChanged)
    Q_PROPERTY(QString uuid READ getUuid NOTIFY uuidChanged)
    Q_PROPERTY(bool interactive READ getInteractive WRITE setInteractive NOTIFY interactiveChanged)


public:
    explicit QMLTextEditor(QDeclarativeItem *parent = 0);

    NoteStore* getNoteStore();
    void setNoteStore(NoteStore *Store);

    int getMinHeight() const;
    void setMinHeight(int height);

    bool getBold() const;
    bool getItalic() const;
    bool getUnderline() const;
    bool getStrikeout() const;
    bool getHighlight() const;
    bool getFixedWidth() const;
    int getFontSize() const;

    QTextCursor textCursor();

    QFont getFont();
    void setFont(QFont font);

    QColor getColor();
    void setColor(QColor color);

    QColor getSelectedTextColor();
    void setSelectedTextColor(QColor color);

    QColor getSelectionColor();
    void setSelectionColor(QColor color);

    bool getFavorite();
    void setFavorite(bool fav);

    QString getUuid() const;

    bool getInteractive() const;
    void setInteractive(bool active);

signals:
    void noteStoreChanged();
    void minHeightChanged();
    void textChanged();
    void cursorPositionChanged(QRect rect);
    void boldChanged();
    void italicChanged();
    void underlineChanged();
    void strikeoutChanged();
    void highlightChanged();
    void fixedWidthChanged();
    void fontSizeChanged();
    void fontChanged();
    void colorChanged();
    void selectedTextColorChanged();
    void selectionColorChanged();
    void favoriteChanged();
    void uuidChanged();
    void interactiveChanged();

public slots:
    void toggleBold();
    void toggleItalic();
    void toggleUnderline();
    void toggleStrikeout();
    void toggleHighlight();
    void toggleFixedWidth();
    void setFontSize(int size);
    void increaseIndent();
    void decreaseIndent();
    void showTestNote();
    void showNote(QString uuid);
    void ignoreNextMouseMoves();
    QString getXml();
    void save();
    void share();
    void deleteNote();

private slots:
    void onWidthChanged();
    void onTextEditHeightChanged(int height);
    void onCursorPositionChanged();
    void onCurrentCharFormatChanged(QTextCharFormat format);
    void onTextChanged();
    //void onContentChanged();
    void onModificationChanged(bool changed);
    void onSaveTimerFired();
    void onEnterPressed();
    void onBackspacePressed();
    void onDeletePressed();
    void onOtherKeyPressed();
    void onNoteRemoved(NoteData* note);


private:
    void formatTitle();
    void removeBullet(QTextCursor *cursor);
    void indentBlock(QTextCursor *cursor, int depth);
    bool blockStartsWithBullet(QTextCursor cursor);
    QGraphicsProxyWidget *proxy;
    TextEditor *textEdit;
    NoteStore *store;
    int qmlWidth;
    int qmlHeight;
    bool bold;
    bool italic;
    bool underline;
    bool strikeout;
    bool highlight;
    bool fixedWidth;
    int fontSize;
    NoteData *currentNote;
    QTimer saveTimer;
    int lastBlockNumber;

};

#endif // QMLTEXTEDITOR_H
