#include <QTextList>
#include <QTextFormat>
#include <QTextListFormat>

#include "style.h"
#include "notecontentxmlhandler.h"


NoteContentXmlHandler::NoteContentXmlHandler(QMLTextEditor *textEditor)
{
    editor = textEditor;
    cursor = editor->textCursor();

    cursor.document()->clear();
    cursor.setPosition(0);

    defaultBlockFormat = cursor.blockFormat();
    defaultCharFormat = cursor.charFormat();

    //defaultCharFormat.setFontPointSize(textEditor->getFont().pointSize());
    defaultCharFormat.setFont(textEditor->getFont());
    defaultFontPixelSize = defaultCharFormat.font().pixelSize();
    cursor.setCharFormat(defaultCharFormat);

    isInternalLink = false;
    isUrlLink = false;
    listDepth = 0;

    bullets.append(QChar(0x2022));
    bullets.append(QChar(0x25e6));
    bullets.append(QChar(0x2219));
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
        format.setFontStyleHint(QFont::Monospace);
        //format.setFontFamily("DejaVu Sans Mono");
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "size:small") {
        QTextCharFormat f = Style::getSmallTextFormat();
        cursor.mergeCharFormat(f);
        return true;
    }

    if (qName == "size:large") {
        QTextCharFormat f = Style::getLargeTextFormat();
        cursor.mergeCharFormat(f);
        return true;
    }

    if (qName == "size:huge") {
        QTextCharFormat f = Style::getHugeTextFormat();
        cursor.mergeCharFormat(f);
        return true;
    }

    if (qName == "link:internal") {
        isInternalLink = true;;
        cursor.mergeCharFormat(Style::getLinkTextFormat());
        return true;
    }

    if (qName == "link:url") {
        isUrlLink = true;
        cursor.mergeCharFormat(Style::getLinkTextFormat());
        return true;
    }

    if (qName == "list") {
        listDepth++;
        return true;
    }

    if (qName == "list-item") {
        QTextBlockFormat f;
        f.setIndent(listDepth);
        f.setTextIndent(-15);
        cursor.setBlockFormat(f);
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
        format.clearBackground();
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
        format.setFontPointSize(defaultCharFormat.fontPointSize());
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "link:internal") {
        QTextCharFormat format = cursor.charFormat();
        format.setAnchor(false);
        format.setForeground(defaultCharFormat.foreground());
        format.setUnderlineStyle(QTextCharFormat::NoUnderline);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "link:url") {
        QTextCharFormat format = cursor.charFormat();
        format.setAnchor(false);
        format.setForeground(defaultCharFormat.foreground());
        format.setUnderlineStyle(QTextCharFormat::NoUnderline);
        cursor.setCharFormat(format);
        return true;
    }

    if (qName == "list") {
        listDepth--;
        // After a list completely ended, we switch back to default block format.
        // We create a new block with this format and (to compensate) skip the next line-break
        if (listDepth == 0) {
            cursor.insertBlock();
            cursor.setBlockFormat(defaultBlockFormat);
            ignoreNextLineBreak = true;
        }
        return true;
    }

    //qDebug() << "WARN: Found unknown tag in <note-content>. Tag: " << qName;
    return true;
}

QString NoteContentXmlHandler::getBullet()
{
    return bullets[listDepth - 1 % 3] + QString(" ");
}

bool NoteContentXmlHandler::characters(const QString &ch)
{
    // Copy string, so we can manipulate it
    QString newCh = ch;

    // Insert bullet if needed (with default formatting)
    if (listDepth > 0 && cursor.positionInBlock() == 0) {
        QTextCharFormat backupFormat = cursor.charFormat();
        cursor.setCharFormat(defaultCharFormat);
        cursor.insertText(getBullet());
        cursor.setCharFormat(backupFormat);
    }

    // Remove line-break if needed
    if (ignoreNextLineBreak && newCh.startsWith("\n")) {
        newCh.remove(0, 1);
        ignoreNextLineBreak = false;
    }

    // If we have a link we need to set the string as href and as text.
    if (isInternalLink || isUrlLink) {
        QTextCharFormat format = cursor.charFormat();
        QString href = isInternalLink ? ("internal://" + newCh) : newCh;
        format.setAnchorHref(href);
        //cursor.setCharFormat(format);
        isInternalLink = false;
        isUrlLink = false;
    }

    // Output all characters
    cursor.insertText(newCh);

    return true;
}

void NoteContentXmlHandler::setErrorString(QString errorString)
{
    errorStr = errorString;
}

QString NoteContentXmlHandler::errorString() const
{
    return errorStr;
}
