#include <QtGui/QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QTimer>

#include "qmlapplicationviewer.h"
#include "qmltexteditor.h"
#include "notelistmodel.h"
#include "tomboystorage.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Export our text editor to QML
    qmlRegisterType<QMLTextEditor>("Conboy", 1, 0, "TextEditor");

    // Holds all notes
    NoteListModel noteList;

    QTime startTime;
    startTime.start();

    TomboyStorage storage;
    QList<QUuid> allUuids = storage.getAllUuids();

    for (int i = 0; i < allUuids.count(); i++) {
        NoteData *note = storage.load(allUuids[i]);
        noteList.append(note);
    }

    qDebug() << "Parsing " << allUuids.length() << " notes took " << startTime.elapsed() << " ms.";

    QmlApplicationViewer viewer;
    viewer.rootContext()->setContextProperty("noteListModel", &noteList);
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    if (app.arguments().contains("--desktop")) {
        viewer.setMainQmlFile(QLatin1String("qml/conboy/DesktopWindow.qml"));
        viewer.showExpanded();
    } else {
        viewer.setMainQmlFile(QLatin1String("qml/conboy/HarmattanWindow.qml"));
        viewer.showFullScreen();
    }

    return app.exec();
}
