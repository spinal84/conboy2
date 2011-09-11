#include <QTextList>
#include <QTextListFormat>
#include <QXmlStreamWriter>
#include <QDebug>

#include "notecontenthelper.h"

NoteContentHelper::NoteContentHelper()
{
}

// Iterate over all fragments of this block
void NoteContentHelper::handleBlock(QTextBlock *block, QXmlStreamWriter *writer)
{
    // Iterate over all fragments of this block
    QTextBlock::Iterator fragmentIter = block->begin();
    while(!fragmentIter.atEnd()) {

        QTextFragment fragment = fragmentIter.fragment();
        if (fragment.isValid()) {

            QTextCharFormat format = fragment.charFormat();

            // TODO: Get all formattings of the fragment and create xml tags
            // Put all fragments after each other
            // After a block, add a newline
            int openTags = 0;

            if (format.fontWeight() == QFont::Bold) {
                writer->writeStartElement("bold");
                openTags++;
            }
            if (format.fontItalic()) {
                writer->writeStartElement("italic");
                openTags++;
            }
            // TODO: We need a mapping between tags and formats
            // This mapping should be used in notecontentxmlhandler.cpp
            // and here.
            // TODO: Is there something like formatA.contains(formatB)? Maybe == ?
            if (format.fontFamily() == "DejaVu Sans Mono") {
                writer->writeStartElement("monospace");
                openTags++;
            }

            writer->writeCharacters(fragment.text());

            for (int i = 0; i < openTags; i++) {
                writer->writeEndElement();
            }

        }

        fragmentIter++;
    }

}

QString NoteContentHelper::qTextDocumentToXmlString(QTextDocument *doc)
{
    QString result;
    QXmlStreamWriter writer(&result);
    writer.writeStartElement("note-content");
    writer.writeAttribute("version", "0.1");

    // Iterate over all blocks
    for (int i = 0; i < doc->blockCount(); i++) {

        QTextBlock block = doc->findBlockByNumber(i);

        QTextList *list = block.textList();
        if (list) {

            if (list->itemNumber(block) == 0) {
                qDebug() << "First item in list";
                writer.writeStartElement("list");
            }

            writer.writeStartElement("list-item");
            QTextBlock item = list->item(list->itemNumber(block));
            handleBlock(&item, &writer);
            writer.writeCharacters("\n");
            writer.writeEndElement();

            if (list->itemNumber(block) +1 == list->count()) {
                qDebug() << "Last item in list";
                writer.writeEndElement();
            }

        } else {
            // For each block, insert a newline
            if (i > 0) {
                writer.writeCharacters("\n");
            }
            handleBlock(&block, &writer);
        }
    }

    // </note-content>
    writer.writeEndElement();

    return result;
}
