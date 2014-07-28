#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QtWidgets>

#include "meshviewer.h"
#include "checkableAction.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	// constructor
    MainWindow(QWidget *parent = 0);
	// deconstructor
    ~MainWindow();

private:
	// show mesh with filename
	// void showMesh(const char * filename);

private slots:
	void showAlignWindow();
	void startPoissonRecon();

private:
    void initWindow();
    void createActions();
    void createMenus();
    void createToolbar();

private:

    // actions
    QAction * openAction;
    QAction * saveAction;
    QAction * saveasAction;
    QAction * exitAction;
    QAction * undoAction;
    QAction * redoAction;
    QAction * showToolbarAction;
    QAction * showStatusAction;
    QAction * showAboutAction;
	QAction * alignMeshes;
	QAction * lightControl;
	QAction * reconAction;

    // menus
    QMenu * fileMenu;
    QMenu * editMenu;
    QMenu * viewMenu;
    QMenu * helpMenu;

    // toolbars
    QToolBar * fileToolbar;
	QToolBar * editToolbar;
	QToolBar * viewToolbar;

	// mesh viewer
	MeshViewer * viewer;
	

};

#endif // MAINWINDOW_H
