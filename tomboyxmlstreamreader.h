#ifndef TOMBOYXMLSTREAMREADER_H
#define TOMBOYXMLSTREAMREADER_H

#include <QXmlStreamReader>

class TomboyXmlStreamReader : public QXmlStreamReader
{
public:
    TomboyXmlStreamReader();
    TomboyXmlStreamReader(QIODevice *device);
    QString innerXml();

private:
    void readInnerXml(QXmlStreamWriter &sw);


};

#endif // TOMBOYXMLSTREAMREADER_H
