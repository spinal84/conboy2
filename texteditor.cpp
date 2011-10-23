#include <QDebug>
#include <QInputContext>

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
    setCursorWidth(2);
    document()->setDocumentMargin(20);
    ignoreNxtMouseMoves = false;
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

void TextEditor::mousePressEvent(QMouseEvent *e)
{
    mousePressPos = e->pos();
    QTextEdit::mousePressEvent(e);
}

void TextEditor::mouseReleaseEvent(QMouseEvent *e)
{
    // If the user didn't move the mouse more then 30px, check if we have a link.
    if ((abs(e->x() - mousePressPos.x()) < 30) && abs(e->y() - mousePressPos.y() < 30)) {
        QString anchor = anchorAt(mousePressPos);
        if (anchor.length() > 0) {
            qDebug() << "DEBUG: Link clicked: " << anchor;
            emit linkClicked(anchor);
        }
    }

    ignoreNxtMouseMoves = false;
    QTextEdit::mouseReleaseEvent(e);
}

void TextEditor::mouseMoveEvent(QMouseEvent *e)
{
    // If ignoreNxtMouseMoves is set, we ignore mouse move events until the next mouse release event.
    if (ignoreNxtMouseMoves) {
        e->accept();
        return;
    }

    QTextEdit::mouseMoveEvent(e);
}

void TextEditor::keyPressEvent(QKeyEvent *e)
{
    switch(e->key()) {
        case Qt::Key_Enter:
        case Qt::Key_Return:    emit enterPressed(); break;
        case Qt::Key_Backspace: emit backspacePressed(); break;
        case Qt::Key_Delete:    emit deletePressed(); break;
        default:                emit otherKeyPressed(); QTextEdit::keyPressEvent(e); return;
    }

    e->accept();
    return;
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

void TextEditor::ignoreNextMouseMoves()
{
    ignoreNxtMouseMoves = true;
}

void TextEditor::inputMethodEvent(QInputMethodEvent *e)
{
//    qDebug() << "";
//    qDebug() << "Preedit: " << e->preeditString();
//    qDebug() << "";

    //QInputContext *ctx = inputContext();
    //qDebug() << "Font italic:" << ctx->font().italic();

    QTextEdit::inputMethodEvent(e);
//    QTextEdit::inputMethodHints();
    //QTextEdit::setInputMethodHints()
}

void TextEditor::dragEnterEvent(QDragEnterEvent *e)
{
    // Disable drag & drop
    e->ignore();
}
