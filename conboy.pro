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

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    qmltexteditor.cpp \
    texteditor.cpp \
    tomboystorage.cpp \
    notelistmodel.cpp \
    notedata.cpp \
    notecontentxmlhandler.cpp \
    tomboyxmlstreamreader.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += \
    qmltexteditor.h \
    texteditor.h \
    tomboystorage.h \
    notelistmodel.h \
    notedata.h \
    notecontentxmlhandler.h \
    tomboyxmlstreamreader.h

OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog
