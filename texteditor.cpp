#include <QDebug>
#include "texteditor.h"

TextEditor::TextEditor(QWidget *parent) :
    QTextEdit(parent)
{
    init();
}

TextEditor::TextEditor(QString text, QWidget *parent) :
    QTextEdit(text, parent)
{
    init();
}

void TextEditor::init()
{
    setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    connect(this, SIGNAL(textChanged()), this, SLOT(resizeHeight()));
}

void TextEditor::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);
    if (e->size().height() != e->oldSize().height()) {
        emit heightChanged(e->size().height());
    }

    resizeHeight();
}

void TextEditor::resizeHeight()
{
    // Resize the widget to make it as big as its content. We want the
    // widget never to display its scroll bars. Instead it should be the content
    // of a QML Flickable
    int docHeight = document()->size().height();
    setMinimumHeight(qMax(docHeight, qmlHeight));
    setMaximumHeight(qMax(docHeight, qmlHeight));
}

void TextEditor::setQmlHeight(int height)
{
    qmlHeight = height;
}
