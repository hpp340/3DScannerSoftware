#-------------------------------------------------
#
# Project created by QtCreator 2014-07-18T19:38:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MainSoftware
TEMPLATE = app

QT += opengl

SOURCES += main.cpp\
        mainwindow.cpp \
    meshviewer.cpp

HEADERS  += mainwindow.h \
    meshviewer.h \
    OpenGLHeader.h

OTHER_FILES +=

RESOURCES += \
    resources.qrc
