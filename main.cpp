#include <QtGui/QApplication>
#include <QDeclarativeEngine>

#include "qmlapplicationviewer.h"
#include "qmltexteditor.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Export our text editor to QML
    qmlRegisterType<QMLTextEditor>("Conboy", 1, 0, "TextEditor");

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);

    if (app.arguments().contains("--desktop")) {
        viewer.setMainQmlFile(QLatin1String("qml/conboy/main_desktop.qml"));
        viewer.showExpanded();
    } else {
        viewer.setMainQmlFile(QLatin1String("qml/conboy/main_harmattan.qml"));
        viewer.showFullScreen();
    }

    return app.exec();
}
