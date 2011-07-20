#ifndef NOTECONTENTXMLHANDLER_H
#define NOTECONTENTXMLHANDLER_H

#include <QXmlDefaultHandler>
#include <QStack>

#include <qmltexteditor.h>

class NoteContentXmlHandler : public QXmlDefaultHandler
{
public:
    NoteContentXmlHandler(QMLTextEditor *editor);
    bool fatalError(const QXmlParseException& exception);
    bool startElement(const QString &namepsaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts);
    bool endElement(const QString &namespaceURI, const QString &localName, const QString &qName);
    bool characters(const QString &ch);
    void setErrorString(QString errorString);
    QString errorString() const;

private:
    QStack<QString> stack;
    QString errorStr;
    QMLTextEditor *editor;
    QTextCursor cursor;
    bool createNextListItem;
    bool listHasEnded;
    QStack<QTextList*> listStack;
    QTextBlockFormat defaultBlockFormat;

};

#endif // NOTECONTENTXMLHANDLER_H
