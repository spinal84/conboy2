#include <QTextList>
#include <QTextListFormat>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QStack>

#include "notecontenthelper.h"

NoteContentHelper::NoteContentHelper()
{
}

// Iterate over all fragments of this block
void NoteContentHelper::handleBlock(QTextBlock *block, QXmlStreamWriter *writer)
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
//            if (format.fontFamily() == "DejaVu Sans Mono") {
//                writer->writeStartElement("monospace");
//                openTags++;
//            }
            QStringList tags = getXmlTags(&format);
            for (int i = 0; i < tags.length(); i++) {
                writer->writeStartElement(tags[i]);
            }

            writer->writeCharacters(fragment.text());

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

QString NoteContentHelper::qTextDocumentToXmlString(QTextDocument *doc)
{
    QString result;
    QXmlStreamWriter writer(&result);
    writer.writeStartElement("note-content");
    writer.writeAttribute("version", "0.1");

    // Iterate over all blocks
    bool listEnded = false;
    QTextList *listRoot = 0;

    QTextFrame *frame = doc->rootFrame();
    QTextFrame::Iterator it;
    for (it = frame->begin(); !it.atEnd(); it++) {

        QTextBlock block = it.currentBlock();

        QTextList *list = block.textList();

        // If the last block was a list ending, we need to close some tags
        if (listEnded) {
            writer.writeEndElement(); // </list>
            writer.writeEndElement(); // </list-item>
            listEnded = false;
        }

        if (list) {
            if (!listRoot) {
                listRoot = list;
            }

            int index = list->itemNumber(block);
            qDebug() << "/// INDEX: " << index;

            // If first and last
            if (index == 0 && list->count() == 1) {
                // First <list> element must be on new line
                if (list == listRoot) {
                    writer.writeCharacters("\n");
                }
                writer.writeStartElement("list");
                listEnded = true;

                writer.writeStartElement("list-item");
                QTextBlock item = list->item(list->itemNumber(block));
                handleBlock(&item, &writer);

                if (list == listRoot) {
                    listRoot = 0;
                } else {
                    writer.writeCharacters("\n");
                }
            }

            // If first item
            else if (index == 0) {
                // First <list> element must be on new line
                if (list == listRoot) {
                    writer.writeCharacters("\n");
                }
                writer.writeStartElement("list");

                writer.writeStartElement("list-item");
                QTextBlock item = list->item(list->itemNumber(block));
                handleBlock(&item, &writer);
                writer.writeCharacters("\n");
            }

            // If in middle
            else if (index > 0 && index + 1 < list->count()) {
                writer.writeEndElement(); // </list-item>

                writer.writeStartElement("list-item");
                QTextBlock item = list->item(list->itemNumber(block));
                handleBlock(&item, &writer);
                writer.writeCharacters("\n");
            }

            // If last item
            else if (index + 1 == list->count()) {
                listEnded = true;
                writer.writeEndElement(); // </list-item>

                writer.writeStartElement("list-item");
                QTextBlock item = list->item(list->itemNumber(block));
                handleBlock(&item, &writer);

                if (list == listRoot) {
                    listRoot = 0;
                } else {
                    writer.writeCharacters("\n");
                }
            }



        } else {
            // For each block, insert a newline
            if (it.currentBlock().blockNumber() > 0) {
                writer.writeCharacters("\n");
            }
            handleBlock(&block, &writer);
        }
    }

    // </note-content>
    writer.writeEndElement();

//    qDebug() << "*************************************";
//    qDebug() << result;
//    qDebug() << "*************************************";

    //return "<note-content>Text <size:large>LARGE</size:large></note-content>";
    return result;
}
