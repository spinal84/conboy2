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
    void keyPressEvent(QKeyEvent *e);
    void setQmlHeight(int height);
    void ignoreNextMouseMoves();
    void inputMethodEvent(QInputMethodEvent *e);
    void dragEnterEvent(QDragEnterEvent *e);
    bool getInteractive() const;
    void setInteractive(bool active);

signals:
    void heightChanged(int height);
    void linkClicked(QString link);
    void enterPressed();
    void deletePressed();
    void backspacePressed();
    void otherKeyPressed();

private slots:
    void resizeHeight();

private:
    int qmlHeight;
    void init();
    bool ignoreNxtMouseMoves;
    QPoint mousePressPos;
    bool interactive;

};

#endif // TEXTEDITOR_H
