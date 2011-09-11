#ifndef NOTECONTENTHELPER_H
#define NOTECONTENTHELPER_H

#include <QTextDocument>

class NoteContentHelper
{
public:
    NoteContentHelper();

    static QString qTextDocumentToXmlString(QTextDocument *doc);

private:
    static void handleBlock(QTextBlock *block, QXmlStreamWriter *writer);


};

#endif // NOTECONTENTHELPER_H
