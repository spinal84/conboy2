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

public:
    explicit QMLTextEditor(QDeclarativeItem *parent = 0);

    int getMinHeight() const;
    void setMinHeight(int height);

    qreal getFontSize() const;
    void setFontSize(qreal size);

signals:
    void minHeightChanged();
    void fontSizeChanged();

public slots:
    void makeBold();

private slots:
    void onWidthChanged();
    void onTextEditHeightChanged(int height);


private:
    QGraphicsProxyWidget *proxy;
    TextEditor *textEdit;
    int qmlWidth;
    int qmlHeight;
    qreal fontSize;

};

#endif // QMLTEXTEDITOR_H
