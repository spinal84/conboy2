#include "notecontentxmlhandler.h"

NoteContentXmlHandler::NoteContentXmlHandler(QMLTextEditor *textEditor)
{
    editor = textEditor;
    cursor = editor->textCursor();

    cursor.document()->clear();
    cursor.setPosition(0);
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
    qDebug() << "<" + qName + ">";

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
        format.setFontPointSize(20);
        cursor.setCharFormat(format);
        return true;
    }

    qDebug() << "WARN: Found unknown tag in <note-content>. Tag: " << qName;
    return true;
}

bool NoteContentXmlHandler::characters(const QString &ch)
{
    // Use cursor to write all text.
    // TODO: Maybe we should use the text block concept of QTextEdit not only \n \n
    cursor.insertText(ch);
    return true;
}

void NoteContentXmlHandler::setErrorString(QString errorStr)
{
    errorStr = errorStr;
}

QString NoteContentXmlHandler::errorString() const
{
    return errorString();
}
