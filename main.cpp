#include <QtGui/QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>

#include "qmlapplicationviewer.h"
#include "qmltexteditor.h"
#include "notelistmodel.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Export our text editor to QML
    qmlRegisterType<QMLTextEditor>("Conboy", 1, 0, "TextEditor");

    // Holds all notes
    NoteListModel noteList;

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("noteListModel", &noteList);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    if (app.arguments().contains("--desktop")) {
        viewer.setMainQmlFile(QLatin1String("qml/conboy/DesktopWindow.qml"));
        //viewer.setMainQmlFile(QLatin1String("qml/conboy/NoteListPage.qml"));
        viewer.showExpanded();
    } else {
        viewer.setMainQmlFile(QLatin1String("qml/conboy/HarmattanWindow.qml"));
        viewer.showFullScreen();
    }

    return app.exec();
}
