#include <QTextList>
#include <QTextFormat>
#include <QTextListFormat>

#include "notecontentxmlhandler.h"


NoteContentXmlHandler::NoteContentXmlHandler(QMLTextEditor *textEditor)
{
    editor = textEditor;
    cursor = editor->textCursor();

    cursor.document()->clear();
    cursor.setPosition(0);

    defaultBlockFormat = cursor.blockFormat();
    defaultCharFormat = cursor.charFormat();

    createNextListItem = false;
    listHasEnded = false;
}

bool NoteContentXmlHandler::fatalError(const QXmlParseException& exception)
{
    qWarning() << "Fatal error on line" << exception.lineNumber()
               << ", column" << exception.columnNumber() << ":"
               << exception.message();

    return false;
}

bool NoteContentXmlHandler::startElement(const QString &namepsaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{
    Q_UNUSED(namepsaceURI);
    Q_UNUSED(localName);
    Q_UNUSED(atts);
    //qDebug() << "<" + qName + ">";

    if (qName == "bold") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontWeight(QFont::Bold);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "italic") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontItalic(true);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "highlight") {
        QTextCharFormat format = cursor.charFormat();
        format.setBackground(QBrush(QColor("yellow")));
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "size:small") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontPointSize(16);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "size:large") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontPointSize(24);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "size:huge") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontPointSize(28);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "list") {
        // The first list item is created implicitly, therefore we
        // do not create another one.
        createNextListItem = false;

        QTextList *list = cursor.currentList();
        QTextListFormat format;
        if (list) {
            format = list->format();
            format.setIndent(format.indent() + 2);
            format.setStyle(QTextListFormat::ListDisc);
        } else {
            format.setIndent(2);
            format.setStyle(QTextListFormat::ListDisc);
        }

        QTextList *newList = cursor.insertList(format);
        listStack.push(newList);
    }

    if (qName == "list-item") {
        QTextList *list = cursor.currentList();

        if (listHasEnded) {
            if (listStack.count() > 1) {
                // Attach next list item to parent list
                listStack.pop();

                cursor.insertBlock();
                listStack.top()->add(cursor.block());

                listHasEnded = false;
                return true;
            }
        }

        if (list) {
            // The first list item is created implicitly, therefore we
            // do not create another one.
            if (createNextListItem == true) {
                cursor.insertBlock();
            }

            // After inserting the first item we have to create the following
            // list item explicitly.
            if (list->count() == 1) {
                createNextListItem = true;
            }

            return true;
        }

        qDebug() << "ERROR: New <list-item> but not in <list>.";
        return false;
    }

    return true;
}

bool NoteContentXmlHandler::endElement(const QString &namespaceURI, const QString &localName, const QString &qName)
{
    if (qName == "bold") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontWeight(QFont::Normal);;
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "italic") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontItalic(false);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "highlight") {
        QTextCharFormat format = cursor.charFormat();
        format.setBackground(QBrush(QColor("white")));
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "size:small" || qName == "size:large" || qName == "size:huge") {
        QTextCharFormat format = cursor.charFormat();
        qDebug() << "FONT SIZE:" << defaultCharFormat.fontPointSize();
        //format.setFontPointSize(defaultCharFormat.fontPointSize());
        format.setFontPointSize(11);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "list") {
        if (listStack.count() > 1) {
            listHasEnded = true;
            return true;
        }

        /* We're on the root list element */
        // Extra check to be save
        if (cursor.currentList() != listStack.top()) {
            qDebug() << "ERROR: Something wrong";
            return false;
        }

        // Add the next block
        cursor.insertBlock();

        // Remove this block from the list. It's now on the root level
        cursor.currentList()->remove(cursor.block());

        // Apply default format, otherwise it's still indented
        cursor.setBlockFormat(defaultBlockFormat);

        // Throw away the last list element
        listStack.pop();

        // TODO: There is an empty line after the list that shouldn't be there.
        // but using cursor.deletePreviousChar(); does not work.

        // TODO: There is also an empty line before each list. Shouldn't be there.
    }

    //qDebug() << "WARN: Found unknown tag in <note-content>. Tag: " << qName;
    return true;
}

bool NoteContentXmlHandler::characters(const QString &ch)
{
    // Use cursor to write all text.
    // TODO: Maybe we should use the text block concept of QTextEdit not only \n \n

    if (listStack.empty()) {
        cursor.insertText(ch);
        return true;
    }

    // If inside a list, remove line-breaks
    if (ch.endsWith("\n")) {
        QString str = ch;
        str.chop(1);
        cursor.insertText(str);
        return true;
    } else {
        cursor.insertText(ch);
        return true;
    }
}

void NoteContentXmlHandler::setErrorString(QString errorString)
{
    errorStr = errorString;
}

QString NoteContentXmlHandler::errorString() const
{
    return errorStr;
}
