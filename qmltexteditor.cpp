#include "qmltexteditor.h"

QMLTextEditor::QMLTextEditor(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    textEdit = new TextEditor("Hello, <i>this</i> <b>is a</b> test.");
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(textEdit);
    proxy->setPos(0, 0);

    connect(this, SIGNAL(widthChanged()), this, SLOT(onWidthChanged()));
    connect(textEdit, SIGNAL(heightChanged(int)), this, SLOT(onTextEditHeightChanged(int)));
    connect(textEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
}

void QMLTextEditor::onTextEditHeightChanged(int height)
{
    setHeight(height);
}

void QMLTextEditor::onWidthChanged()
{
    textEdit->resize(width(), textEdit->height());
}

int QMLTextEditor::getMinHeight() const
{
    return textEdit->minimumHeight();
}

void QMLTextEditor::setMinHeight(int height)
{
    textEdit->setQmlHeight(height);
    textEdit->setMinimumHeight(height);
}

void QMLTextEditor::onCursorPositionChanged()
{
    QRect rect = textEdit->cursorRect();
    emit cursorPositionChanged(rect);
}

qreal QMLTextEditor::getFontSize() const
{
    return fontSize;
}

void QMLTextEditor::setFontSize(qreal size)
{
    if (fontSize != size) {
        fontSize = size;
        textEdit->setFont(QFont("Arial", fontSize));
    }
}

void QMLTextEditor::makeBold()
{
    textEdit->setFontWeight(QFont::Bold);
}
