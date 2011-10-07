#include <QTextList>
#include <QTextListFormat>
#include <QXmlStreamWriter>
#include <QDebug>
#include <QStack>

#include "style.h"
#include "notecontenthelper.h"


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

            // Add open tags
            QStringList tags = Style::getXmlTags(&format);
            for (int i = 0; i < tags.length(); i++) {
                writer->writeStartElement(tags[i]);
            }

            // If in list, remove bullets
            QString text = fragment.text();
            if (inList) {
                if (Style::startsWithBullet(text)) {
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

//    qDebug() << "*************************************";
//    qDebug() << result;
//    qDebug() << "*************************************";

//    return "<note-content>Text <size:large>LARGE</size:large></note-content>";
    return result;
}
