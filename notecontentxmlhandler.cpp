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

    if (qName == "strikethrough") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontStrikeOut(true);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "underline") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontUnderline(true);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "monospace") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontFamily("DejaVu Sans Mono");
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
        listHasEnded = false;

        QTextList *list = cursor.currentList();
        QTextList *newList;
        QTextListFormat format;
        if (list) {
            format = list->format();
            format.setIndent(format.indent() + 2);
            format.setStyle(QTextListFormat::ListDisc);
            newList = cursor.insertList(format);
        } else {
            format.setIndent(2);
            format.setStyle(QTextListFormat::ListDisc);
            newList = cursor.createList(format);
        }

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

                //listHasEnded = false;
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
    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)

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

    if (qName == "strikethrough") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontStrikeOut(false);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "underline") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontUnderline(false);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "monospace") {
        QTextCharFormat format = cursor.charFormat();
        format.setFontFamily(defaultCharFormat.fontFamily());
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

        listHasEnded = true;

        if (listStack.count() > 1) {
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
    // The outer-most <list> item ends with a "\n". Remove it.
    if (listHasEnded && listStack.isEmpty()) {
        if (ch.startsWith("\n")) {
            QString str = ch;
            str.remove(0, 1);
            cursor.insertText(str);
            return true;
        }
    }

    // Normal text, not inside a list
    if (!listHasEnded && listStack.isEmpty()) {
        cursor.insertText(ch);
        return true;
    }

    // If inside a list, remove all line-breaks
    if (!listStack.isEmpty()) {
        if (ch.endsWith("\n")) {
            QString str = ch;
            str.chop(1);
            cursor.insertText(str);
        } else {
            cursor.insertText(ch);
        }
        return true;
    }

    qDebug() << "ERROR: Unhandled situation. String is:" << ch;
    return false;
}

void NoteContentXmlHandler::setErrorString(QString errorString)
{
    errorStr = errorString;
}

QString NoteContentXmlHandler::errorString() const
{
    return errorStr;
}
