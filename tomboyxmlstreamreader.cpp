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
    QXmlStreamWriter writer(&ret);
    writer.setAutoFormatting(false);

    readInnerXml(writer);

    return ret;
}

void TomboyXmlStreamReader::readInnerXml(QXmlStreamWriter &writer)
{
    if(tokenType() == QXmlStreamReader::StartElement)
    {
    // copy the start tag
        writer.writeStartElement(qualifiedName().toString());

        // copy all the attributes
        foreach(QXmlStreamAttribute a, attributes())
        {
            writer.writeAttribute(a.qualifiedName().toString(), a.value().toString());
        }

        // Copy children recursively (maybe make this a behavior flag?)
        //  Maybe the user can specify the depth of the traversal?
        int t;
        while((t = readNext()) != QXmlStreamReader::EndElement &&
              t != QXmlStreamReader::EndDocument &&
              t != QXmlStreamReader::Invalid)
        {
            if(t == QXmlStreamReader::Characters)
                writer.writeCharacters(text().toString());
            else
                readInnerXml(writer);
        }

        // Close the element
        writer.writeEndElement();
    }
}
