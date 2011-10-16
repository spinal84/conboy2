#include <QtGui/QApplication>
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QTimer>
#include <QScopedPointer>
#include <QSortFilterProxyModel>

#include "notestore.h"
#include "qmlapplicationviewer.h"
#include "qmltexteditor.h"
#include "notelistmodel.h"
#include "tomboystorage.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    // Export our text editor to QML
    qmlRegisterType<QMLTextEditor>("Conboy", 1, 0, "TextEditor");
    qmlRegisterType<NoteData>();

    // Holds all notes
    NoteStore noteStore;

    // Model to visualize notes
    NoteListModel noteListModel(&noteStore);

    // Proxy model used for filtering
    QSortFilterProxyModel filteredNoteListModel;
    filteredNoteListModel.setSourceModel(&noteListModel);
    filteredNoteListModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    filteredNoteListModel.setFilterRole(NoteListModel::TitleRole);

    filteredNoteListModel.setSortCaseSensitivity(Qt::CaseInsensitive);
    filteredNoteListModel.setSortRole(NoteListModel::LastChangeDateRole);
    filteredNoteListModel.setDynamicSortFilter(true);
    filteredNoteListModel.sort(0, Qt::DescendingOrder);

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
