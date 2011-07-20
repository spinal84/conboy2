#ifndef QMLTEXTEDITOR_H
#define QMLTEXTEDITOR_H

#include <QDeclarativeItem>
#include <QGraphicsProxyWidget>
#include <QTextEdit>
#include "texteditor.h"

class QMLTextEditor : public QDeclarativeItem
{
    Q_OBJECT
    Q_PROPERTY(int minHeight READ getMinHeight WRITE setMinHeight NOTIFY minHeightChanged)
    Q_PROPERTY(qreal fontSize READ getFontSize WRITE setFontSize NOTIFY fontSizeChanged)
    Q_PROPERTY(bool bold READ getBold NOTIFY boldChanged)

public:
    explicit QMLTextEditor(QDeclarativeItem *parent = 0);

    int getMinHeight() const;
    void setMinHeight(int height);

    qreal getFontSize() const;
    void setFontSize(qreal size);

    bool getBold() const;
    //void setBold(bool isBold);


signals:
    void minHeightChanged();
    void fontSizeChanged();
    void textChanged();
    void cursorPositionChanged(QRect rect);
    void boldChanged();

public slots:
    void toggleBold();
    void increaseIndent();
    void decreaseIndent();


private slots:
    void onWidthChanged();
    void onTextEditHeightChanged(int height);
    void onCursorPositionChanged();
    void onCurrentCharFormatChanged(QTextCharFormat format);
    void showTestNote();

private:
    QGraphicsProxyWidget *proxy;
    TextEditor *textEdit;
    int qmlWidth;
    int qmlHeight;
    qreal fontSize;
    bool bold;

};

#endif // QMLTEXTEDITOR_H
