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
    Q_PROPERTY(int minHeight READ getMinHeight WRITE setMinHeight NOTIFY minHeightChanged)
    Q_PROPERTY(bool bold READ getBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ getItalic NOTIFY italicChanged)
    Q_PROPERTY(bool underline READ getUnderline NOTIFY underlineChanged)
    Q_PROPERTY(bool strikeout READ getStrikeout NOTIFY strikeoutChanged)
    Q_PROPERTY(bool highlight READ getHighlight NOTIFY highlightChanged)
    Q_PROPERTY(bool fixedWidth READ getFixedWidth NOTIFY fixedWidthChanged)
    Q_PROPERTY(QFont font READ getFont WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor selectedTextColor READ getSelectedTextColor WRITE setSelectedTextColor NOTIFY selectedTextColorChanged)
    Q_PROPERTY(QColor selectionColor READ getSelectionColor WRITE setSelectionColor NOTIFY selectionColorChanged)
    Q_PROPERTY(bool favorite READ getFavorite WRITE setFavorite NOTIFY favoriteChanged)


public:
    explicit QMLTextEditor(QDeclarativeItem *parent = 0);

    int getMinHeight() const;
    void setMinHeight(int height);

    bool getBold() const;
    bool getItalic() const;
    bool getUnderline() const;
    bool getStrikeout() const;
    bool getHighlight() const;
    bool getFixedWidth() const;

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


signals:
    void minHeightChanged();
    void fontSizeChanged();
    void textChanged();
    void cursorPositionChanged(QRect rect);
    void boldChanged();
    void italicChanged();
    void underlineChanged();
    void strikeoutChanged();
    void highlightChanged();
    void fixedWidthChanged();
    void fontChanged();
    void colorChanged();
    void selectedTextColorChanged();
    void selectionColorChanged();
    void favoriteChanged();

public slots:
    void toggleBold();
    void toggleItalic();
    void toggleUnderline();
    void toggleStrikeout();
    void toggleHighlight();
    void toggleFixedWidth();
    void increaseIndent();
    void decreaseIndent();
    void showTestNote();
    void showNote(NoteData *note);
    void ignoreNextMouseMoves();
    QString getXml();
    void save();
    void share();

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

private:
    void formatTitle();
    void removeBullet(QTextCursor *cursor);
    void indentBlock(QTextCursor *cursor, int depth);
    bool blockStartsWithBullet(QTextCursor cursor);
    QGraphicsProxyWidget *proxy;
    TextEditor *textEdit;
    int qmlWidth;
    int qmlHeight;
    qreal fontSize;
    bool bold;
    bool italic;
    bool underline;
    bool strikeout;
    bool highlight;
    bool fixedWidth;
    NoteData *currentNote;
    QTimer saveTimer;
    int lastBlockNumber;

};

#endif // QMLTEXTEDITOR_H
