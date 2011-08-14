#ifndef TEXTEDITOR_H
#define TEXTEDITOR_H

#include <QTextEdit>
#include <QResizeEvent>

class TextEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit TextEditor(QWidget *parent = 0);
    explicit TextEditor(QString text, QWidget *parent = 0);
    void resizeEvent(QResizeEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void setQmlHeight(int height);
    void ignoreNextMouseMoves();

signals:
    void heightChanged(int height);
    void linkClicked(QString link);

private slots:
    void resizeHeight();

private:
    int qmlHeight;
    void init();
    bool ignoreNxtMouseMoves;
    QPoint mousePressPos;

};

#endif // TEXTEDITOR_H
