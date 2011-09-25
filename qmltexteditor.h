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

    Q_PROPERTY(QFont font READ getFont WRITE setFont NOTIFY fontChanged)
    Q_PROPERTY(QColor color READ getColor WRITE setColor NOTIFY colorChanged)
    Q_PROPERTY(QColor selectedTextColor READ getSelectedTextColor WRITE setSelectedTextColor NOTIFY selectedTextColorChanged)
    Q_PROPERTY(QColor selectionColor READ getSelectionColor WRITE setSelectionColor NOTIFY selectionColorChanged)


public:
    explicit QMLTextEditor(QDeclarativeItem *parent = 0);

    int getMinHeight() const;
    void setMinHeight(int height);

    bool getBold() const;

    QTextCursor textCursor();

    QFont getFont();
    void setFont(QFont font);

    QColor getColor();
    void setColor(QColor color);

    QColor getSelectedTextColor();
    void setSelectedTextColor(QColor color);

    QColor getSelectionColor();
    void setSelectionColor(QColor color);

signals:
    void minHeightChanged();
    void fontSizeChanged();
    void textChanged();
    void cursorPositionChanged(QRect rect);
    void boldChanged();
    void fontChanged();
    void colorChanged();
    void selectedTextColorChanged();
    void selectionColorChanged();

public slots:
    void toggleBold();
    void increaseIndent();
    void decreaseIndent();
    void showTestNote();
    void showNote(NoteData *note);
    void ignoreNextMouseMoves();
    QString getXml();

private slots:
    void onWidthChanged();
    void onTextEditHeightChanged(int height);
    void onCursorPositionChanged();
    void onCurrentCharFormatChanged(QTextCharFormat format);
    void onTextChanged();
    //void onContentChanged();
    void onModificationChanged(bool changed);
    void onSaveTimerFired();

private:
    QGraphicsProxyWidget *proxy;
    TextEditor *textEdit;
    int qmlWidth;
    int qmlHeight;
    qreal fontSize;
    bool bold;
    void formatTitle();
    NoteData *currentNote;
    QTimer saveTimer;
    int lastBlockNumber;

};

#endif // QMLTEXTEDITOR_H
