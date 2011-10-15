#include "style.h"

const QString Style::bullet1 = QChar(0x2022) + QString(" ");
const QString Style::bullet2 = QChar(0x25e6) + QString(" ");
const QString Style::bullet3 = QChar(0x2219) + QString(" ");

const int Style::smallFontPixelSize   = 20;
const int Style::defaultFontPixelSize = 25;
const int Style::largeFontPixelSize   = 30;
const int Style::hugeFontPixelSize    = 35;

Style::Style()
{
}

QTextCharFormat Style::getDefaultCharFormat()
{
    QTextCharFormat f;
    f.setFontFamily("Nokia Pure Text");
    f.setProperty(QTextCharFormat::FontPixelSize, defaultFontPixelSize);
    f.setFontWeight(QFont::Normal);
    f.setFontItalic(false);
    f.setFontUnderline(false);
    f.setFontStrikeOut(false);
    f.clearBackground();
    f.clearForeground();
    return f;
}

QTextCharFormat Style::getTitleCharFormat()
{
    QTextCharFormat f = getDefaultCharFormat();
    f.setProperty(QTextCharFormat::FontPixelSize, hugeFontPixelSize);
    f.setFontUnderline(true);
    f.setForeground(QBrush(QColor("blue")));
    return f;
}

QTextCharFormat Style::getFixedWidthFormat()
{
    QTextCharFormat f;
    f.setFontFamily("Monospace");
    return f;
}

QTextCharFormat Style::getNormalFont()
{
    QTextCharFormat f;
    f.setFontFamily("Nokia Pure Text");
    return f;
}

QTextCharFormat Style::getSmallTextFormat()
{
    QTextCharFormat f;
    f.setProperty(QTextCharFormat::FontPixelSize, smallFontPixelSize);
    return f;
}

QTextCharFormat Style::getLargeTextFormat()
{
    QTextCharFormat f;
    f.setProperty(QTextCharFormat::FontPixelSize, largeFontPixelSize);
    return f;
}

QTextCharFormat Style::getHugeTextFormat()
{
    QTextCharFormat f;
    f.setProperty(QTextCharFormat::FontPixelSize, hugeFontPixelSize);
    return f;
}

QTextCharFormat Style::getLinkTextFormat()
{
    QTextCharFormat f;
    f.setAnchor(true);
    f.setForeground(QBrush(QColor("blue")));
    f.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    return f;
}

bool Style::startsWithBullet(QString str)
{
    return (str.startsWith(bullet1) || str.startsWith(bullet2) || str.startsWith(bullet3));
}

QString Style::getBullet(int depth)
{
    int num = (depth - 1 ) % 3;
    switch(num) {
        case 0: return bullet1;
        case 1: return bullet2;
        case 2: return bullet3;
        default: return bullet1;
    }
}

QStringList Style::getXmlTags(QTextCharFormat *format)
{
    QStringList tags;
    QFont font = format->font();

    if (format->fontWeight() == QFont::Bold) {
        tags.append("bold");
    }

    if (format->fontItalic()) {
        tags.append("italic");
    }

    if (format->background().color() == "yellow") {
        tags.append("highlight");
    }

    if (format->fontStrikeOut()) {
        tags.append("strikethrough");
    }

    if (format->fontStyleHint() == QFont::Monospace) {
        tags.append("monospace");
    }

    if (font.pixelSize() == smallFontPixelSize) {
        tags.append("size:small");
    }

    if (font.pixelSize() == largeFontPixelSize) {
        tags.append("size:large");
    }

    if (font.pixelSize() == hugeFontPixelSize) {
        tags.append("size:huge");
    }

    // Simply underlined, internal link or url link
    if (format->fontUnderline()) {
        if (format->isAnchor()) {
            if (format->anchorHref().startsWith("internal://")) {
                tags.append("link:internal");
            } else {
                tags.append("link:url");
            }
        } else {
            tags.append("underline");
        }
    }

    return tags;
}
