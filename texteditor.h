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
    void setQmlHeight(int height);

signals:
    void heightChanged(int height);

public slots:

private slots:
    void resizeHeight();

private:
    int qmlHeight;
    void init();

};

#endif // TEXTEDITOR_H
