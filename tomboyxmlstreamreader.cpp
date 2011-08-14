#include "tomboyxmlstreamreader.h"

TomboyXmlStreamReader::TomboyXmlStreamReader()
{
}

TomboyXmlStreamReader::TomboyXmlStreamReader(QIODevice *device)
    : QXmlStreamReader(device)
{
}

QString TomboyXmlStreamReader::innerXml()
{
    QString ret;
    QXmlStreamWriter sw(&ret);
    sw.setAutoFormatting(false);

    readInnerXml(sw);

    return ret;
}

void TomboyXmlStreamReader::readInnerXml(QXmlStreamWriter &sw)
{
    if(tokenType() == QXmlStreamReader::StartElement)
    {
    // copy the start tag
        sw.writeStartElement(qualifiedName().toString());

        // copy all the attributes
        foreach(QXmlStreamAttribute a, attributes())
        {
            sw.writeAttribute(a.qualifiedName().toString(), a.value().toString());
        }

        // Copy children recursively (maybe make this a behavior flag?)
        //  Maybe the user can specify the depth of the traversal?
        int t;
        while((t = readNext()) != QXmlStreamReader::EndElement &&
              t != QXmlStreamReader::EndDocument &&
              t != QXmlStreamReader::Invalid)
        {
            if(t == QXmlStreamReader::Characters)
                sw.writeCharacters(text().toString());
            else
                readInnerXml(sw);
        }

        // Close the element
        sw.writeEndElement();
    }
}
