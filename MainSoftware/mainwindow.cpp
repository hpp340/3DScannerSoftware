#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	viewer = new MeshViewer();
	setCentralWidget(viewer);
    initWindow();
    createActions();
    createToolbar();
    createMenus();
	showMesh("genus2ply.ply");
}

MainWindow::~MainWindow()
{
}

void MainWindow::initWindow()
{
}

void MainWindow::showMesh(const char * filename)
{
	viewer->loadFile(filename);
}

void MainWindow::createActions()
{
    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon(":/icons/images/open.ico"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a mesh file"));
    // todo:connect

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":icons/images/save.ico"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save to a mesh file"));
    // todo:connect

    saveasAction = new QAction(tr("&Save As"), this);
    saveasAction->setIcon(QIcon(":icons/images/saveas.ico"));
    saveasAction->setShortcut(QKeySequence::SaveAs);
    saveasAction->setStatusTip(tr("Save as"));
    // todo:connect

    exitAction = new QAction(tr("&Exit"), this);
    exitAction->setShortcut(tr("Ctrl+q"));
    exitAction->setStatusTip(tr("Exit"));
    // todo:connect

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(tr("Ctrl+z"));
    undoAction->setStatusTip(tr("Undo"));
    // todo:connect

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcut(tr("Ctrl+r"));
    redoAction->setStatusTip(tr("Redo"));
    // todo:connect

    showToolbarAction = new QAction(tr("Show &Toolbar"), this);
    showToolbarAction->setShortcut(tr("Ctrl+t"));
    showToolbarAction->setStatusTip(tr("Show Toolbar"));
    // todo:connect

    showStatusAction = new QAction(tr("Show &Status Bar"), this);
    showStatusAction->setShortcut(tr("Ctrl+s"));
    showStatusAction->setStatusTip(tr("Show Status Bar"));
    // todo:connect

    showAboutAction = new QAction(tr("Ab&out"), this);
    showAboutAction->setShortcut(tr("Ctrl+o"));
    showAboutAction->setStatusTip(tr("About"));
    // todo:connect
}

void MainWindow::createToolbar()
{
    fileToolbar = addToolBar(tr("&File"));
    fileToolbar->addAction(openAction);
    fileToolbar->addAction(saveAction);
    fileToolbar->addAction(saveasAction);
}

void MainWindow::createMenus()
{
    // filemenu
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveasAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    // editmenu
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);

    // viewmenu
    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(showToolbarAction);
    viewMenu->addAction(showStatusAction);

    // helpmenu
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(showAboutAction);
}
