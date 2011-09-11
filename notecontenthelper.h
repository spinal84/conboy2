#ifndef NOTECONTENTHELPER_H
#define NOTECONTENTHELPER_H

#include <QTextDocument>
#include <QTextBlock>
#include <QTextList>
#include <QStringList>

class NoteContentHelper
{
public:
    NoteContentHelper();

    static QString qTextDocumentToXmlString(QTextDocument *doc);

private:
    static void handleBlock(QTextBlock *block, QXmlStreamWriter *writer);
    static void handleList(QTextList *list, QXmlStreamWriter *writer);
    static void handleListItem(QTextBlock *list, QXmlStreamWriter *writer);
    static QStringList getXmlTags(QTextCharFormat *format);


};

#endif // NOTECONTENTHELPER_H
