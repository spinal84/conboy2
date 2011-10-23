#include <QTextList>
#include <QTextListFormat>
#include <QtXml/QXmlSimpleReader>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QTextListFormat>
#include <QPrinter>

//#include <maemo-meegotouch-interfaces/shareuiinterface.h>

#include "qmltexteditor.h"
#include "notecontentxmlhandler.h"
#include "notecontenthelper.h"
#include "style.h"

QMLTextEditor::QMLTextEditor(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    textEdit = new TextEditor();
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(textEdit);
    proxy->setPos(0, 0);

    currentNote = 0;
    store = 0;
    lastBlockNumber = -1;
    saveTimer.setSingleShot(true);

    connect(this, SIGNAL(widthChanged()), this, SLOT(onWidthChanged()));
    connect(textEdit, SIGNAL(heightChanged(int)), this, SLOT(onTextEditHeightChanged(int)));
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
    connect(textEdit, SIGNAL(cursorPositionChanged()), this, SLOT(onCursorPositionChanged()));
    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)), this, SLOT(onCurrentCharFormatChanged(QTextCharFormat)));
    connect(&saveTimer, SIGNAL(timeout()), this, SLOT(onSaveTimerFired()));
    textEdit->document()->setModified(false);
    connect(textEdit->document(), SIGNAL(modificationChanged(bool)), this, SLOT(onModificationChanged(bool)));
    connect(textEdit, SIGNAL(enterPressed()), this, SLOT(onEnterPressed()));
    connect(textEdit, SIGNAL(backspacePressed()), this, SLOT(onBackspacePressed()));
    connect(textEdit, SIGNAL(deletePressed()), this, SLOT(onDeletePressed()));
    connect(textEdit, SIGNAL(otherKeyPressed()), this, SLOT(onOtherKeyPressed()));
}

NoteStore* QMLTextEditor::getNoteStore()
{
    return store;
}

void QMLTextEditor::setNoteStore(NoteStore *noteStore)
{
    if (store == 0) {
        store = noteStore;
        connect(store, SIGNAL(noteRemoved(NoteData*)), this, SLOT(onNoteRemoved(NoteData*)));
    }
}

void QMLTextEditor::onNoteRemoved(NoteData* note)
{
    if (currentNote == note) {
        currentNote = 0;
        textEdit->clear();
        saveTimer.stop();
    }
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

    // If first line, set title style
    if (c.block().blockNumber() == 0) {
        textEdit->setCurrentCharFormat(Style::getTitleCharFormat());
    }

    emit textChanged();
}


void QMLTextEditor::onEnterPressed()
{
    QTextCursor cursor = textEdit->textCursor();
    qDebug() << "MARGIN " << cursor.blockFormat().leftMargin();
    qDebug() << "INDENT " << cursor.blockFormat().textIndent();

    if (cursor.blockNumber() == 0) {
        // Set to default font formattting
        QTextCharFormat format = Style::getDefaultCharFormat();
        cursor.setCharFormat(format);
        cursor.insertBlock();
        cursor.select(QTextCursor::BlockUnderCursor);
        cursor.setCharFormat(format);
        return;
    }

    // Move cursor to start of block to check for bullets
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
    QString selection = cursor.selectedText();


    // Check if "*" or "-"
    if (selection == "* " || selection == "- ") {
        cursor.removeSelectedText();
        increaseIndent();
    }

    // Check if bullet
    cursor = textEdit->textCursor();
    if (blockStartsWithBullet(cursor)) {

        // If last list item is empty, close list
        if (cursor.block().length() <= 3) {
            indentBlock(&cursor, 0);
        } else {
            cursor.insertBlock();
            indentBlock(&cursor, cursor.blockFormat().indent());
        }
        return;
    }

    // Normal text
    textEdit->textCursor().insertBlock();
}

void QMLTextEditor::onBackspacePressed()
{
    // Decrease indent if near a bullet
    QTextCursor cursor = textEdit->textCursor();
    if (cursor.positionInBlock() <= 2 && blockStartsWithBullet(cursor)) {
        decreaseIndent();
        return;
    }
    cursor.deletePreviousChar();
}

void QMLTextEditor::onDeletePressed()
{
    // TODO: Once this code should run on devices with "Delete" key, finish this method.
    //       If DEL is pressed on the end of a list-item, it needs to pull up the next line

    // Decrease indent if near a bullet
    QTextCursor cursor = textEdit->textCursor();
    if (cursor.positionInBlock() <= 1 && blockStartsWithBullet(cursor)) {
        decreaseIndent();
        return;
    }
    cursor.deleteChar();
}

void QMLTextEditor::onOtherKeyPressed()
{
    QTextCursor cursor = textEdit->textCursor();

    // If directly before or after a bullet, place cursor correctly before inserting
    if (cursor.positionInBlock() < 2 && blockStartsWithBullet(cursor)) {
        cursor.movePosition(QTextCursor::StartOfBlock);
        cursor.movePosition(QTextCursor::Right, QTextCursor::MoveAnchor, 2);
        textEdit->setTextCursor(cursor);
    }
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
    textEdit->setFontWeight(bold ? QFont::Normal : QFont::Bold);
}

void QMLTextEditor::toggleItalic()
{
    textEdit->setFontItalic(!italic);
}

void QMLTextEditor::toggleUnderline()
{
    textEdit->setFontUnderline(!underline);
}

void QMLTextEditor::toggleStrikeout()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat f;
    f.setFontStrikeOut(!strikeout);
    cursor.mergeCharFormat(f);
    textEdit->setTextCursor(cursor);
}

void QMLTextEditor::toggleHighlight()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat f = cursor.charFormat();
    QColor yellow = QColor("yellow");
    if (f.background().color() == yellow) {
        f.clearBackground();
    } else {
        f.setBackground(QBrush(yellow));
    }
    cursor.setCharFormat(f);
    textEdit->setTextCursor(cursor);
}

void QMLTextEditor::toggleFixedWidth()
{
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat f = cursor.charFormat();
    if (f.fontFamily() == "Monospace") {
        f = Style::getNormalFont();
    } else {
        f = Style::getFixedWidthFormat();
    }
    cursor.mergeCharFormat(f);
    textEdit->setTextCursor(cursor);
}

void QMLTextEditor::setFontSize(int size)
{
    QTextCursor cursor = textEdit->textCursor();
    QTextCharFormat f;
    switch (size) {
        case 0: f = Style::getSmallTextFormat(); break;
        case 1: f = Style::getNormalTextFormat(); break;
        case 2: f = Style::getLargeTextFormat(); break;
        case 3: f = Style::getHugeTextFormat(); break;
        default: f = Style::getNormalTextFormat(); break;
    }

    cursor.mergeCharFormat(f);
    textEdit->setTextCursor(cursor);
}

bool QMLTextEditor::getBold() const
{
    return bold;
}

bool QMLTextEditor::getItalic() const
{
    return italic;
}

bool QMLTextEditor::getUnderline() const
{
    return underline;
}

bool QMLTextEditor::getStrikeout() const
{
    return strikeout;
}

bool QMLTextEditor::getHighlight() const
{
    return highlight;
}

bool QMLTextEditor::getFixedWidth() const
{
    return fixedWidth;
}

int QMLTextEditor::getFontSize() const
{
    return fontSize;
}

void QMLTextEditor::onCurrentCharFormatChanged(QTextCharFormat format)
{
    bool b = (format.fontWeight() == QFont::Bold);
    bool i = format.fontItalic();
    bool u = format.fontUnderline();
    bool s = format.fontStrikeOut();
    bool h = format.background() == QBrush(QColor("yellow"));
    // TODO: Which font do we use? Adjust 'mono' or use styleHint
    bool f = format.fontFamily().contains("mono", Qt::CaseInsensitive);
    int size = Style::getFontSize(&format);

    // TODO: Maybe put in setters
    if (bold != b) {
        bold = b;
        emit boldChanged();
    }

    if (italic != i) {
        italic = i;
        emit italicChanged();
    }
    if (underline != u) {
        underline = u;
        emit underlineChanged();
    }

    if (strikeout != s) {
        strikeout = s;
        emit strikeoutChanged();
    }

    if (highlight != h) {
        highlight = h;
        emit highlightChanged();
    }

    if (fixedWidth != f) {
        fixedWidth = f;
        emit fixedWidthChanged();
    }

    if (fontSize != size) {
        fontSize = size;
        emit fontSizeChanged();
    }
}

QTextCursor QMLTextEditor::textCursor()
{
    return textEdit->textCursor();
}

bool QMLTextEditor::blockStartsWithBullet(QTextCursor cursor)
{
    cursor.movePosition(QTextCursor::StartOfBlock);
    cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
    return Style::startsWithBullet(cursor.selectedText());
}

void QMLTextEditor::removeBullet(QTextCursor *cursor)
{
    if (blockStartsWithBullet(*cursor)) {
        cursor->movePosition(QTextCursor::StartOfBlock);
        cursor->movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, 2);
        cursor->removeSelectedText();
        cursor->movePosition(QTextCursor::StartOfBlock);
    }
}

void QMLTextEditor::indentBlock(QTextCursor *cursor, int depth)
{
    QTextBlockFormat blockFormat;
    QTextCharFormat tmpFormat = cursor->charFormat();
    QTextCursor tmpCursor = *cursor;

    removeBullet(cursor);

    // Remove bullet and set indent to 0
    if (depth <= 0) {
        blockFormat = cursor->blockFormat();
        blockFormat.setIndent(0);
        blockFormat.setTextIndent(0);
        cursor->setBlockFormat(blockFormat);

    // Remove old bullet, add new bullet, set indent
    } else {

        // Add new bullet with default format
        cursor->movePosition(QTextCursor::StartOfBlock);
        cursor->setCharFormat(Style::getDefaultCharFormat());
        cursor->insertText(Style::getBullet(depth));

        // Set indent
        blockFormat = cursor->blockFormat();
        blockFormat.setIndent(depth);
        blockFormat.setTextIndent(-15);
        cursor->setBlockFormat(blockFormat);

        // After inserting the bullet switch format of the current cursor back to as it was before
        tmpCursor.setCharFormat(tmpFormat);
        textEdit->setTextCursor(tmpCursor);
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
    cursor.setCharFormat(Style::getTitleCharFormat());
}

void QMLTextEditor::showNote(QString uuid)
{
    NoteData *note = store->findNote(uuid);
    qDebug() << "INFO: Showing note: " << note->getTitle();

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

    if (currentNote) {
        disconnect(currentNote, SIGNAL(favoriteChanged()), this, SIGNAL(favoriteChanged()));
    }

    currentNote = note;

    connect(currentNote, SIGNAL(favoriteChanged()), this, SIGNAL(favoriteChanged()));

    // Emit the signal, so that the editor UI can update it's display
    // TODO: Maybe a more general noteChanged() makes sense
    emit favoriteChanged();
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
//    NoteData *note = new NoteData();
//    note->setTitle("The Title");
//    note->setContent(content);
//    showNote(note);
}

QFont QMLTextEditor::getFont()
{
    return textEdit->font();
}

void QMLTextEditor::setFont(QFont font)
{
    if (font.pixelSize() > 0) {
        if (textEdit->font() != font) {
            qDebug() << "INFO: Setting editor font. Size: " << font.pixelSize();
            textEdit->setFont(font);
            emit fontChanged();
        }
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

QString QMLTextEditor::getXml()
{
    QTextCursor cursor = textCursor();
    cursor.movePosition(QTextCursor::Start);

    QTextDocument *doc = cursor.document();

    QString tmp = NoteContentHelper::qTextDocumentToXmlString(doc);
    // TODO: Remove. Only for debugging
    qDebug() << tmp;
    return tmp;
}

bool QMLTextEditor::getFavorite()
{
    if (currentNote) {
        return currentNote->getFavorite();
    }

    return false;
}

void QMLTextEditor::setFavorite(bool fav)
{
    if (currentNote) {
        currentNote->setFavorite(fav);
    }
}

void QMLTextEditor::share()
{
    /*
     * TODO: We need to export the PDF (or HTML, or TXT), then tell Tracker to
     * index it. Then share it. While exporting and indexing we should display
     * spinner or something...
     */
    qDebug() << "TODO: Implement sharing";
    /*
    QPrinter printer;
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("/home/developer/MyDocs/bla.pdf");
    textEdit->print(&printer);

    QStringList items;
    items.append("file:///home/developer/MyDocs/bla.pdf");

    // Create a interface object
    ShareUiInterface interface("com.nokia.ShareUi");

    // Check if interface is valid
    if (interface.isValid()) {
        interface.share(items);
    } else {
        qDebug() << "ERROR: Invalid sharing interface";
        return;
    }
    */
}

void QMLTextEditor::deleteNote()
{
    if (currentNote) {
        store->del(currentNote);
    }
}
