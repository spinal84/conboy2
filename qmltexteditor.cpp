#include <QTextList>
#include <QTextListFormat>

#include "qmltexteditor.h"

QMLTextEditor::QMLTextEditor(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    textEdit = new TextEditor("Hello, <i>this</i> <b>is a</b> test.");
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(textEdit);
    proxy->setPos(0, 0);

    bold = false;

    //textEdit->setAutoFormatting(QTextEdit::AutoBulletList);

    connect(this, SIGNAL(widthChanged()), this, SLOT(onWidthChanged()));
    connect(textEdit, SIGNAL(heightChanged(int)), this, SLOT(onTextEditHeightChanged(int)));
    connect(textEdit, SIGNAL(textChanged()), this, SIGNAL(textChanged()));
    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(onCurrentCharFormatChanged(QTextCharFormat)));
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

void QMLTextEditor::toggleBold()
{
    if (bold) {
        textEdit->setFontWeight(QFont::Normal);
    } else {
        textEdit->setFontWeight(QFont::Bold);
    }
}

bool QMLTextEditor::getBold() const
{
    return bold;
}

//void QMLTextEditor::setBold(bool isBold)
//{
//    if (bold != isBold) {
//        bold = isBold;
//        emit boldChanged();
//    }
//}

void QMLTextEditor::onCurrentCharFormatChanged(QTextCharFormat format)
{
    bold = (format.fontWeight() == QFont::Bold);
    emit boldChanged();
}

void QMLTextEditor::increaseIndent()
{
    QTextCursor cursor = textEdit->textCursor();

    QTextListFormat listFormat;
    QTextList *list = cursor.currentList();

    if (list) {
        listFormat = list->format();
        listFormat.setIndent(listFormat.indent() + 1);
    }

    listFormat.setStyle(QTextListFormat::ListDisc);
    cursor.createList(listFormat);
}

// TODO: List item needs to get attached to parent list
// if current list has a parent list
void QMLTextEditor::decreaseIndent()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextListFormat listFormat;
    QTextList *list = cursor.currentList();
    QTextBlock block = cursor.block();

    if (list) {
        listFormat = list->format();
        listFormat.setIndent(listFormat.indent() - 1);
        listFormat.setStyle(QTextListFormat::ListDisc);
        list->setFormat(listFormat);
        list->remove(block);
    }
}
