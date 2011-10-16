# Add more folders to ship with the application, here
folder_01.source = qml/conboy
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

QT += xml

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xEE54320D

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
## TODO: Enable booster once a bug is fixed. Currently QML cannot access
##       C++ data if this is enabled. Other than that we only need to set
##       --type=d in conboy.desktop
## CONFIG += qdeclarative-boostable

# Share UI setup
CONFIG += shareuiinterface-maemo-meegotouch

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    qmltexteditor.cpp \
    texteditor.cpp \
    tomboystorage.cpp \
    notelistmodel.cpp \
    notedata.cpp \
    notecontentxmlhandler.cpp \
    tomboyxmlstreamreader.cpp \
    notestore.cpp \
    notecontenthelper.cpp \
    style.cpp

HEADERS += \
    qmltexteditor.h \
    texteditor.h \
    tomboystorage.h \
    notelistmodel.h \
    notedata.h \
    notecontentxmlhandler.h \
    tomboyxmlstreamreader.h \
    notestore.h \
    notecontenthelper.h \
    style.h

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog


contains(MEEGO_EDITION,harmattan) {
    desktopfile.files = $${TARGET}.desktop
    desktopfile.path = /usr/share/applications
    INSTALLS += desktopfile
}

contains(MEEGO_EDITION,harmattan) {
    icon.files = conboy.png
    icon.path = /usr/share/icons/hicolor/80x80/apps
    INSTALLS += icon
}








