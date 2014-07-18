#-------------------------------------------------
#
# Project created by QtCreator 2014-07-16T14:26:10
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

qtHaveModule(opengl)
{
    QT  += opengl

    TARGET = mainWindow
    TEMPLATE = app


    SOURCES += main.cpp\
            mainwindow.cpp \
        showmesh.cpp \
    PlyCloud.cpp

    HEADERS  += mainwindow.h \
        showmesh.h \
    headers/PlyCloud.h \
    headers/Point.h

    FORMS    += mainwindow.ui

    RESOURCES += \
        menu.qrc

    win32 {
    LIBS += "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\GlU32.Lib" \
        "C:\Program Files (x86)\Windows Kits\8.1\Lib\winv6.3\um\x64\OpenGL32.Lib"
    }
}
