#include "mainwindow.h"
#include "showmesh.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    showMesh gl;
    w.show();
    gl.show();
    return a.exec();
}
