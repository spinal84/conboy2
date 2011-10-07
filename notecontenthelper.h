#ifndef NOTECONTENTHELPER_H
#define NOTECONTENTHELPER_H

#include <QTextDocument>
#include <QTextBlock>
#include <QTextList>
#include <QStringList>
#include <QXmlStreamWriter>

class NoteContentHelper
{
public:
    NoteContentHelper();

    static QString qTextDocumentToXmlString(QTextDocument *doc);

private:
    static void handleBlock(QTextBlock *block, bool inList, QXmlStreamWriter *writer);
    static void changeDepthOfList(QXmlStreamWriter *writer, int depthChange);

};



#endif // NOTECONTENTHELPER_H
