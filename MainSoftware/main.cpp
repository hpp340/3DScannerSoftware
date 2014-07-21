#include "mainwindow.h"
#include "alignWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication scannerApp(argc, argv);
    MainWindow mainWin;
    mainWin.setGeometry(100, 100, mainWin.sizeHint().width(), mainWin.sizeHint().height());
    mainWin.resize(mainWin.sizeHint());
    mainWin.showMaximized();
	
	alignWindow *registration = new alignWindow();
	registration->show();
    return scannerApp.exec();
}
