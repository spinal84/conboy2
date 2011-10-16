#include <QtGui/QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QTimer>
#include <QScopedPointer>

#include "notestore.h"
#include "qmlapplicationviewer.h"
#include "qmltexteditor.h"
#include "notelistmodel.h"
#include "tomboystorage.h"
#include "notesortfilterproxymodel.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    // Export our text editor to QML
    qmlRegisterType<QMLTextEditor>("Conboy", 1, 0, "TextEditor");
    qmlRegisterType<NoteData>();
    qmlRegisterType<NoteStore>();

    // Holds all notes
    NoteStore noteStore;

    // Model to visualize notes
    NoteListModel noteListModel(&noteStore);

    // Proxy model used for filtering
    NoteSortFilterProxyModel filteredNoteListModel;
    filteredNoteListModel.setSourceModel(&noteListModel);

    // Expose objects to QML
    viewer->rootContext()->setContextProperty("noteListModel", &filteredNoteListModel);
    viewer->rootContext()->setContextProperty("noteStore", &noteStore);

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    if (app->arguments().contains("--desktop")) {
        viewer->setMainQmlFile(QLatin1String("qml/conboy/DesktopWindow.qml"));
        viewer->showExpanded();
    } else {
        viewer->setMainQmlFile(QLatin1String("qml/conboy/HarmattanWindow.qml"));
        viewer->showFullScreen();
    }

    return app->exec();
}
