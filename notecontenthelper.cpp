#include <QTextList>
#include <QTextListFormat>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QStack>

#include "notecontenthelper.h"

QString NoteContentHelper::bullet1 = QChar(0x2022) + QString(" ");
QString NoteContentHelper::bullet2 = QChar(0x25e6) + QString(" ");
QString NoteContentHelper::bullet3 = QChar(0x2219) + QString(" ");

NoteContentHelper::NoteContentHelper()
{    
}

// Iterate over all fragments of this block
void NoteContentHelper::handleBlock(QTextBlock *block, bool inList, QXmlStreamWriter *writer)
{
    // The title is saved without formattings
    if (block->blockNumber() == 0) {
        writer->writeCharacters(block->text());
        return;
    }

    // Iterate over all fragments of this block
    QTextBlock::Iterator fragmentIter = block->begin();
    while(!fragmentIter.atEnd()) {

        QTextFragment fragment = fragmentIter.fragment();
        if (fragment.isValid()) {

            QTextCharFormat format = fragment.charFormat();

            // TODO: Get all formattings of the fragment and create xml tags
            // Put all fragments after each other
            // After a block, add a newline

            // TODO: We need a mapping between tags and formats
            // This mapping should be used in notecontentxmlhandler.cpp
            // and here.
            // TODO: Is there something like formatA.contains(formatB)? Maybe == ?

            // Add open tags
            QStringList tags = getXmlTags(&format);
            for (int i = 0; i < tags.length(); i++) {
                writer->writeStartElement(tags[i]);
            }

            // If in list, remove bullets
            QString text = fragment.text();
            if (inList) {
                if (text.startsWith(bullet1) || text.startsWith(bullet2) || text.startsWith(bullet3)) {
                    text.remove(0, 2);
                }
            }

            // Write text
            writer->writeCharacters(text);

            // Add close tags
            for (int i = 0; i < tags.length(); i++) {
                writer->writeEndElement();
            }

        }

        fragmentIter++;
    }
}


// TODO: Implements all used formats at a single place. It's not nice
// to depend on things like fontsize == 16 to figure out a certain tag.
// I hope there is something like format.contains(other_format).
QStringList NoteContentHelper::getXmlTags(QTextCharFormat *format)
{
    QStringList tags;

    if (format->fontWeight() == QFont::Bold) {
        tags.append("bold");
    }

    if (format->fontItalic()) {
        tags.append("italic");
    }

    if (format->background().color() == "yellow") {
        tags.append("highlight");
    }

    if (format->fontStrikeOut()) {
        tags.append("strikethrough");
    }

    if (format->fontStyleHint() == QFont::Monospace) {
        tags.append("monospace");
    }

    if (format->fontPointSize() == 16) {
        tags.append("size:small");
    }

    if (format->fontPointSize() == 24) {
        tags.append("size:large");
    }

    if (format->fontPointSize() == 28) {
        tags.append("size:huge");
    }

    // Simply underlined, internal link or url link
    if (format->fontUnderline()) {
        if (format->isAnchor()) {
            if (format->anchorHref().startsWith("internal://")) {
                tags.append("link:internal");
            } else {
                tags.append("link:url");
            }
        } else {
            tags.append("underline");
        }
    }

    return tags;
}

// Positive number: Increase depth
// Negative number: Decrease depth
void NoteContentHelper::changeDepthOfList(QXmlStreamWriter *writer, int depthChange)
{
    // Increase indent/depth
    if (depthChange > 0) {
        writer->writeStartElement("list");
        for (int i = 1; i < depthChange; i++) {
            writer->writeStartElement("list-item");
            writer->writeStartElement("list");
        }
        return;
    }

    // Decrease indent/depth
    if (depthChange < 0) {
        writer->writeEndElement();
        for (int i = 0; i > depthChange; i--) {
            writer->writeEndElement();
            writer->writeEndElement();
        }
        return;
    }
}

QString NoteContentHelper::qTextDocumentToXmlString(QTextDocument *doc)
{
    QString result;
    QXmlStreamWriter writer(&result);
    writer.writeStartElement("note-content");
    writer.writeAttribute("version", "0.1");

    // Iterate over all blocks
    QTextFrame *frame = doc->rootFrame();
    QTextFrame::Iterator it;
    for (it = frame->begin(); !it.atEnd(); it++) {

        QTextBlock block = it.currentBlock();
        int depth = block.blockFormat().indent();

        if (depth > 0) {

            // If start of new list
            bool isFirstListItem = (block.previous().blockFormat().indent() == 0);
            bool isLastListItem = (block.next().blockFormat().indent() == 0);
            int nextDepth = block.next().blockFormat().indent();

            // If it's the first item of a new root-list. Line-break and add <list>
            if (isFirstListItem) {
                writer.writeCharacters("\n");
                writer.writeStartElement("list");
                // If we start at depth > 1, we need more open tags
                for (int i = 1; i < depth; i++) {
                    writer.writeStartElement("list-item");
                    writer.writeStartElement("list");
                }
            }

            // Always add <list-item> and the content of the block
            writer.writeStartElement("list-item");
            handleBlock(&block, true, &writer);

            // If it's the last item inside the root-list. </list-item></list>
            if (isLastListItem) {
                for (int i = 0; i < depth; i++) {
                    writer.writeEndElement();
                    writer.writeEndElement();
                }
                continue;
            }

            // If we are in the middle of the list, add a new-line and check
            // if the list just got deeper or shallower.
            writer.writeCharacters("\n");

            if (nextDepth == depth) {
                writer.writeEndElement(); // </list-item>
            } else {
                changeDepthOfList(&writer, nextDepth - depth);
            }


        } else {
            // If we're not inside a list: For each block, insert a newline
            if (it.currentBlock().blockNumber() > 0) {
                writer.writeCharacters("\n");
            }
            handleBlock(&block, false, &writer);
        }

    }

    // </note-content>
    writer.writeEndElement();

    qDebug() << "*************************************";
    qDebug() << result;
    qDebug() << "*************************************";

    return "<note-content>Text <size:large>LARGE</size:large></note-content>";
    //return result;
}
