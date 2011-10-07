#ifndef STYLE_H
#define STYLE_H

#include <QTextCharFormat>

class Style
{
public:
    Style();

    static QTextCharFormat getDefaultCharFormat();
    static QTextCharFormat getTitleCharFormat();
    static QTextCharFormat getSmallTextFormat();
    static QTextCharFormat getLargeTextFormat();
    static QTextCharFormat getHugeTextFormat();
    static QTextCharFormat getLinkTextFormat();

    static bool startsWithBullet(QString str);
    static QString getBullet(int depth);
    static QStringList getXmlTags(QTextCharFormat *format);

private:
    const static QString bullet1;
    const static QString bullet2;
    const static QString bullet3;

    const static int smallFontPixelSize;
    const static int defaultFontPixelSize;
    const static int largeFontPixelSize;
    const static int hugeFontPixelSize;
};

#endif // STYLE_H
