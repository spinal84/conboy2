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

public:
    explicit QMLTextEditor(QDeclarativeItem *parent = 0);

    int getMinHeight() const;
    void setMinHeight(int height);

signals:
    void minHeightChanged();

public slots:

private slots:
    void onWidthChanged();
    void onTextEditHeightChanged(int height);


private:
    QGraphicsProxyWidget *proxy;
    TextEditor *textEdit;
    int qmlWidth;
    int qmlHeight;

};

#endif // QMLTEXTEDITOR_H
