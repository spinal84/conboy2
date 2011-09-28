#include <QTextList>
#include <QTextListFormat>
#include <QtXml/QXmlSimpleReader>
#include <QXmlStreamWriter>
#include <QDebug>

#include <QTextListFormat>

#include "qmltexteditor.h"
#include "notecontentxmlhandler.h"
#include "notecontenthelper.h"

QMLTextEditor::QMLTextEditor(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    textEdit = new TextEditor();
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(textEdit);
    proxy->setPos(0, 0);

    bold = false;
    currentNote = 0;
    lastBlockNumber = -1;
    saveTimer.setSingleShot(true);

    bullets.append(QChar(0x2022) + QString(" "));
    bullets.append(QChar(0x25e6) + QString(" "));
    bullets.append(QChar(0x2219) + QString(" "));

    connect(this, SIGNAL(widthChanged()), this, SLOT(onWidthChanged()));
    connect(textEdit, SIGNAL(heightChanged(int)), this, SLOT(onTextEditHeightChanged(int)));
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(onCurrentCharFormatChanged(QTextCharFormat)));
    connect(&saveTimer, SIGNAL(timeout()), this, SLOT(onSaveTimerFired()));
    textEdit->document()->setModified(false);
    connect(textEdit->document(), SIGNAL(modificationChanged(bool)), this, SLOT(onModificationChanged(bool)));
}

// TODO: ModificationChanged() is not called repeatedly. Therefore this method is only called
// if modification really changed from false to true (or the other way around).
// That means that while typing we save every 4 seconds. Using something like onContentChanged,
// we would postpone saving until the user is not typing for 4 secons. I'd prefere that.
void QMLTextEditor::onModificationChanged(bool changed)
{
    qDebug() << "Mod changed to:" << changed;
    if (changed) {
        saveTimer.start(4000);
    } else {
        saveTimer.stop();
    }
}

void QMLTextEditor::onTextChanged()
{
    QTextCursor c = textEdit->textCursor();
    if (c.block().blockNumber() == 0) {
        QTextCharFormat titleFormat;
        titleFormat.setFontPointSize(28);
        titleFormat.setFontUnderline(true);
        titleFormat.setForeground(QBrush(QColor("blue")));
        textEdit->setCurrentCharFormat(titleFormat);
    }

    emit textChanged();
}



void QMLTextEditor::onSaveTimerFired()
{
    qDebug() << "SAVE TIMER FIRED";
    if (textEdit->document()->isModified()) {
        qDebug() << "SAVE NOTE !!!!!!";
        QTextCursor cursor = textEdit->textCursor();
        cursor.movePosition(QTextCursor::Start);
        cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);
        QString title = cursor.selectedText();
        QDateTime now = QDateTime::currentDateTime();
        currentNote->setTitle(title);
        currentNote->setContent(getXml());
        currentNote->setLastChangeDate(now);
        currentNote->setLastMetadataChangeDate(now);
        currentNote->save();
        textEdit->document()->setModified(false);
    }
}

void QMLTextEditor::save()
{
    saveTimer.stop();
    onSaveTimerFired();
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
    QTextCursor cursor = textEdit->textCursor();
    int blockNumber = cursor.block().blockNumber();
    if (lastBlockNumber == 0 && blockNumber != 0) {
        // TODO: Check if title is unique. If not show warning.
    }
    lastBlockNumber = blockNumber;

    QRect rect = textEdit->cursorRect();
    emit cursorPositionChanged(rect);
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

void QMLTextEditor::onCurrentCharFormatChanged(QTextCharFormat format)
{
    bold = (format.fontWeight() == QFont::Bold);
    emit boldChanged();
}

QTextCursor QMLTextEditor::textCursor()
{
    return textEdit->textCursor();
}

void QMLTextEditor::removeBullet(QTextCursor *cursor)
{
    cursor->movePosition(QTextCursor::StartOfBlock);
    cursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
    QString s = cursor->selectedText();
    qDebug() << "SELECTED TEXT: " << s;
    if (s.startsWith(bullets[0]) || s.startsWith(bullets[1]) || s.startsWith(bullets[2])) {
        cursor->removeSelectedText();
        cursor->movePosition(QTextCursor::StartOfBlock);
    }
}

void QMLTextEditor::indentBlock(QTextCursor *cursor, int depth)
{
    QTextBlockFormat format;

    // Remove bullet and set indent to 0
    if (depth <= 0) {
        removeBullet(cursor);
        format = cursor->blockFormat();
        format.setIndent(0);
        cursor->setBlockFormat(format);

    // Remove old bullet, add new bullet, set indent
    } else {
        removeBullet(cursor);

        // Add new bullet
        cursor->movePosition(QTextCursor::StartOfBlock);
        cursor->insertText(bullets[(depth-1) % 3]);

        // Set indent
        format = cursor->blockFormat();
        format.setIndent(depth);
        cursor->setBlockFormat(format);
    }
}

void QMLTextEditor::increaseIndent()
{
    QTextCursor cursor = textEdit->textCursor();
    indentBlock(&cursor, cursor.blockFormat().indent() + 1);
}

void QMLTextEditor::decreaseIndent()
{
    QTextCursor cursor = textEdit->textCursor();
    indentBlock(&cursor, cursor.blockFormat().indent() - 1);
}

void QMLTextEditor::formatTitle()
{
    // Set the note title to big/underlined/blue
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start);
    cursor.movePosition(QTextCursor::EndOfLine, QTextCursor::KeepAnchor);

    QTextCharFormat titleFormat;
    titleFormat.setFontPointSize(28);
    titleFormat.setFontUnderline(true);
    titleFormat.setForeground(QBrush(QColor("blue")));
    cursor.setCharFormat(titleFormat);
}

void QMLTextEditor::showNote(NoteData *note)
{
    QXmlSimpleReader xmlReader;
    QXmlInputSource source;
    source.setData(note->getContent());

    NoteContentXmlHandler handler(this);
    xmlReader.setContentHandler(&handler);
    xmlReader.setErrorHandler(&handler);

    textCursor().beginEditBlock();
    if (!xmlReader.parse(&source)) {
        qDebug() << "ERROR: Parsing content failed";
    }

    formatTitle();
    textCursor().endEditBlock();
    textEdit->document()->setModified(false);

    currentNote = note;
}

void QMLTextEditor::showTestNote()
{
    /*
     * Demo <note-content> for testing purpose
     */
    QString content = "<note-content version=\"0.1\">The Title\n\n"
            "This is some text. Next is a line break.\n"
            "And now a link to <link:internal>Qt parameter</link:internal> and so on.\n"
            "External link <link:url>http://heise.de</link:url> and stop.\n"
            "Yea. Next empty line.\n\n"
            "Ok. Now some formatting: <bold>bold</bold> <italic>italic</italic> <highlight>hightlight</highlight> <size:large>large</size:large> <size:small>small</size:small> <size:huge>huge</size:huge> <bold><italic>bolditalic</italic></bold> <bold><highlight>highlightbold</highlight></bold> <italic><highlight>italichighlight</highlight></italic> <bold><italic><highlight>bolditalichighlight</highlight></italic></bold>\n\n"
            "<monospace>And</monospace> <underline>now</underline> <strikethrough>some</strikethrough> bullets\n"
            "<list><list-item dir=\"ltr\">One\n"
            "</list-item><list-item dir=\"ltr\">Start <bold>Two</bold> End\n"
            "</list-item><list-item dir=\"ltr\">Start <bold><italic>TwoAndAHalf</italic></bold> End\n"
            "</list-item><list-item dir=\"ltr\">TwoAndSomeMore\n"
            "<list><list-item dir=\"ltr\"><underline>Two One</underline>\n"
            "</list-item><list-item dir=\"ltr\">Two Two\n"
            "<list><list-item dir=\"ltr\">Two Two One\n"
            "</list-item></list></list-item><list-item dir=\"ltr\">Two Three\n"
            "<list><list-item dir=\"ltr\">Two Three One\n"
            "</list-item><list-item dir=\"ltr\">Two Three Two\n"
            "</list-item></list></list-item><list-item dir=\"ltr\">Two Four\n"
            "</list-item></list></list-item><list-item dir=\"ltr\">Three\n"
            "</list-item><list-item dir=\"ltr\">Four</list-item></list>\n"
            "Now we continue normally...\n"
            "A list with only one item on level 1\n"
            "<list><list-item dir=\"ltr\"><underline>One</underline></list-item></list>\n"
            "End\n"
            "<list><list-item>Level 1\n"
            "<list><list-item><list><list-item>Level 3\n"
            "</list-item></list></list-item></list></list-item><list-item>Level 1</list-item></list>\n"
            "END\n"
            "</note-content>";

    // Create dummy note and show it
    NoteData *note = new NoteData();
    note->setTitle("The Title");
    note->setContent(content);
    showNote(note);
}

QFont QMLTextEditor::getFont()
{
    return textEdit->font();
}

void QMLTextEditor::setFont(QFont font)
{
    if (textEdit->font() != font) {
        textEdit->setFont(font);
        emit fontChanged();
    }
}

QColor QMLTextEditor::getColor()
{
    return textEdit->palette().text().color();
}

void QMLTextEditor::setColor(QColor color)
{
    QPalette palette = textEdit->palette();
    if (palette.text().color() != color) {
        palette.setColor(QPalette::Text, color);
        textEdit->setPalette(palette);
        emit colorChanged();
    }
}

QColor QMLTextEditor::getSelectedTextColor()
{
    return textEdit->palette().highlightedText().color();
}

void QMLTextEditor::setSelectedTextColor(QColor color)
{
    QPalette palette = textEdit->palette();
    if (palette.highlightedText().color() != color) {
        palette.setColor(QPalette::HighlightedText, color);
        textEdit->setPalette(palette);
        emit selectedTextColorChanged();
    }
}

QColor QMLTextEditor::getSelectionColor()
{
    return textEdit->palette().highlight().color();
}

void QMLTextEditor::setSelectionColor(QColor color)
{
    QPalette palette = textEdit->palette();
    if (palette.highlight().color() != color) {
        palette.setColor(QPalette::Highlight, color);
        textEdit->setPalette(palette);
        emit selectionColorChanged();
    }
}

void QMLTextEditor::ignoreNextMouseMoves()
{
    textEdit->ignoreNextMouseMoves();
}

// TODO: This should go into a dedicated class
QString QMLTextEditor::getXml()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start);

    QTextDocument *doc = cursor.document();

    QString tmp = NoteContentHelper::qTextDocumentToXmlString(doc);
    qDebug() << tmp;
    return tmp;
}




