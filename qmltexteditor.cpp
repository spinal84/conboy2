#include <QTextList>
#include <QTextListFormat>
#include <QtXml/QXmlSimpleReader>
#include <QDebug>

#include <QTextListFormat>

#include "qmltexteditor.h"
#include "notecontentxmlhandler.h"

QMLTextEditor::QMLTextEditor(QDeclarativeItem *parent) :
    QDeclarativeItem(parent)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    textEdit = new TextEditor();
    proxy = new QGraphicsProxyWidget(this);
    proxy->setWidget(textEdit);
    proxy->setPos(0, 0);

    bold = false;

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

QTextCursor QMLTextEditor::textCursor()
{
    return textEdit->textCursor();
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

//    QTextCursor cursor = textEdit->textCursor();
//    QTextBlockFormat defaultBlockFormat = cursor.blockFormat();
//    cursor.insertText("Hallo, das ist ein Test. Jetzt kommt eine Liste:\n");

//    QTextListFormat format;
//    format.setIndent(2);
//    format.setStyle(QTextListFormat::ListDisc);
//    cursor.createList(format);
//    cursor.insertText("Erstes Item");


//    cursor.insertBlock();
//    cursor.currentList()->remove(cursor.block());
//    cursor.setBlockFormat(defaultBlockFormat);

//    cursor.insertText("Wieder normal");

//    return;
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

    //qDebug() << textEdit->toHtml();
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
            "<list><list-item dir=\"ltr\">Two One\n"
            "</list-item><list-item dir=\"ltr\">Two Two\n"
            "<list><list-item dir=\"ltr\">Two Two One\n"
            "</list-item></list></list-item><list-item dir=\"ltr\">Two Three\n"
            "<list><list-item dir=\"ltr\">Two Three One\n"
            "</list-item><list-item dir=\"ltr\">Two Three Two\n"
            "</list-item></list></list-item><list-item dir=\"ltr\">Two Four\n"
            "</list-item></list></list-item><list-item dir=\"ltr\">Three\n"
            "</list-item><list-item dir=\"ltr\">Four</list-item></list>\n"
            "Now we continue normally...\n"
            "</note-content>";

    // Create dummy note and show it
    NoteData *note = new NoteData();
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

// color = QPalette::Text
// selectedTextColor = QPalette:HighlightedText
// selectionColor = QPalette:Highlight
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


