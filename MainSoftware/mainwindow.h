#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QtWidgets>

#include "meshviewer.h"
#include "FullFuncMeshViewer.h"
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
	void showPoints();
	void showWireframe();
	void showFlatlines();
	void showFlat();
	void showSmooth();

private:
    void initWindow();
    void createActions();
    void createMenus();
    void createToolbar();

private slots:
	void setModePoints();
	void setModeWireframe();
	void startPoissonRecon();
	void startTrim();

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
	// edit action
	QAction * reconAction;
	QAction * trimAction;

	// control draw mode
	QAction * viewPoints;
	QAction * viewWireframe;
	QAction * viewFlatlines;
	QAction * viewFlat;
	QAction * viewSmooth;
	QActionGroup * drawModeGroup;
	checkableAction * showTexture;

	// select and delete actions
	checkableAction * selectAction;
	QAction * clearAction;
	QAction * deleteAction;

    // menus
    QMenu * fileMenu;
    QMenu * editMenu;
    QMenu * viewMenu;
    QMenu * helpMenu;

    // toolbars
    QToolBar * fileToolbar;
	QToolBar * editToolbar;
	QToolBar * viewToolbar;
	QToolBar * deleteToolbar;

	// mesh viewer
	FullFuncMeshViewer * viewer;
	

};

#endif // MAINWINDOW_H
