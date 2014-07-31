#include "mainwindow.h"
#include "alignWindow.h"
#include "PoissonRecon.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	viewer = new MeshViewer();
	setCentralWidget(viewer);
    initWindow();
    createActions();
    createToolbar();
    createMenus();
	// showMesh("testx.ply");
}

MainWindow::~MainWindow()
{
}

void MainWindow::initWindow()
{
}

//void MainWindow::showMesh(const char * filename)
//{
//	//viewer->loadFile(filename);
//}

void MainWindow::createActions()
{
    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon(":/icons/images/open.ico"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a mesh file"));
	connect(openAction, SIGNAL(triggered()), viewer, SLOT(openMesh()));
    // todo:connect

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":icons/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save to a mesh file"));
	connect(saveAction, SIGNAL(triggered()), viewer, SLOT(saveMesh()));
    // todo:connect

    saveasAction = new QAction(tr("&Save As"), this);
    saveasAction->setIcon(QIcon(":icons/images/saveas.png"));
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

	alignMeshes = new QAction(tr("Align Two &Mesh"), this);
	alignMeshes->setShortcut(tr("Ctrl+M"));
	alignMeshes->setIcon(QIcon(":/icons/images/align.png"));
	connect(alignMeshes, SIGNAL(triggered()), this, SLOT(showAlignWindow()));

	lightControl = new checkableAction(this);
	lightControl->setText(tr("&Light Control"));
	lightControl->setIcon(QIcon(":/icons/images/light.png"));
	lightControl->setCheckable(true);
	lightControl->setChecked(true);
	connect(lightControl, SIGNAL(actionCheck()), viewer, SLOT(turnOnLight()));
	connect(lightControl, SIGNAL(actionUncheck()), viewer, SLOT(turnOffLight()));

	reconAction = new QAction(tr("Poison Surface &Reconstruction"), this);
	reconAction->setIcon(QIcon(":/icons/images/recon.png"));
	connect(reconAction, SIGNAL(triggered()), this, SLOT(startPoissonRecon()));

	viewPoints = new QAction(tr("&Points"), this);
	viewPoints->setIcon(QIcon(":/icons/images/points.png"));
	viewPoints->setText(tr("Draw Points"));
	viewPoints->setStatusTip(tr("Points"));
	viewPoints->setCheckable(true);
	viewPoints->setChecked(false);
	connect(viewPoints, SIGNAL(triggered()), this, SLOT(showPoints()));
	connect(viewer, SIGNAL(setDrawModePoints()), this, SLOT(setModePoints()));

	viewWireframe = new QAction(tr("&Wireframe"), this);
	viewWireframe->setIcon(QIcon(":/icons/images/wireframe.png"));
	viewWireframe->setText(tr("Draw Wireframe"));
	viewWireframe->setStatusTip(tr("Wireframe"));
	viewWireframe->setCheckable(true);
	viewWireframe->setChecked(false);
	connect(viewWireframe, SIGNAL(triggered()), this, SLOT(showWireframe()));
	connect(viewer, SIGNAL(setDrawModeWireframe()), this, SLOT(setModeWireframe()));

	viewFlatlines = new QAction(tr("Flat&lines"), this);
	viewFlatlines->setIcon(QIcon(":/icons/images/flatlines.png"));
	viewFlatlines->setText(tr("Draw Flatlines"));
	viewFlatlines->setStatusTip(tr("Flatlines"));
	viewFlatlines->setCheckable(true);
	viewFlatlines->setChecked(false);
	connect(viewFlatlines, SIGNAL(triggered()), this, SLOT(showFlatlines()));

	viewFlat = new QAction(tr("&Flat"), this);
	viewFlat->setIcon(QIcon(":/icons/images/flat.png"));
	viewFlat->setText(tr("Draw Flat"));
	viewFlat->setStatusTip(tr("Flat"));
	viewFlat->setCheckable(true);
	viewFlat->setChecked(false);
	connect(viewFlat, SIGNAL(triggered()), this, SLOT(showFlat()));

	viewSmooth = new QAction(tr("&Smooth"), this);
	viewSmooth->setIcon(QIcon(":/icons/images/smooth.png"));
	viewSmooth->setText(tr("Draw Smooth"));
	viewSmooth->setStatusTip(tr("Smooth"));
	viewSmooth->setCheckable(true);
	viewSmooth->setChecked(false);
	connect(viewSmooth, SIGNAL(triggered()), this, SLOT(showSmooth()));
}

void MainWindow::createToolbar()
{
    fileToolbar = addToolBar(tr("&File"));
    fileToolbar->addAction(openAction);
    fileToolbar->addAction(saveAction);
    fileToolbar->addAction(saveasAction);

	editToolbar = addToolBar(tr("&Edit"));
	editToolbar->addAction(alignMeshes);
	editToolbar->addAction(reconAction);

	drawModeGroup = new QActionGroup(this);
	drawModeGroup->addAction(viewPoints);
	drawModeGroup->addAction(viewWireframe);
	drawModeGroup->addAction(viewFlatlines);
	drawModeGroup->addAction(viewFlat);
	drawModeGroup->addAction(viewSmooth);

	viewToolbar = addToolBar(tr("&View"));
	viewToolbar->addAction(lightControl);
	viewToolbar->addAction(viewPoints);
	viewToolbar->addAction(viewWireframe);
	viewToolbar->addAction(viewFlatlines);
	viewToolbar->addAction(viewFlat);
	viewToolbar->addAction(viewSmooth);
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

// slots
void MainWindow::showAlignWindow()
{
	alignWindow * registration = new alignWindow();
	registration->show();
}

void MainWindow::startPoissonRecon()
{
	std::cout << "MainWindow:startPoissonRecon.." << std::endl;
	PoissonRecon * surfaceRecon = new PoissonRecon();
	//surfaceRecon->acceptPointCloud(viewer->getMesh());
	std::cout << "MainWindow:filename" << std::endl;
	std::string tempFilename = viewer->getFilename();
	printf("%s\n", tempFilename);
	surfaceRecon->acceptPointCloudFromFile(tempFilename);
	//surfaceRecon->startRecon< 2, PlyVertex< Real >, false >();
	surfaceRecon->startRecon< 2, PlyValueVertex< Real >, false >();
	//viewer->acceptMesh(surfaceRecon->getSurface());
	viewer->loadFile("reconOutput.ply");
}

void MainWindow::showPoints()
{
	std::cout << "MainWindow:showPoints" << std::endl;
	viewer->setDrawMode(DRAW_MODE::POINTS);
}

void MainWindow::showWireframe()
{
	std::cout << "MainWindow:showWireframe" << std::endl;
	viewer->setDrawMode(DRAW_MODE::WIREFRAME);
}

void MainWindow::showFlatlines()
{
	std::cout << "MainWindow:showFlatlines" << std::endl;
	viewer->setDrawMode(DRAW_MODE::FLATLINES);
}

void MainWindow::showFlat()
{
	std::cout << "MainWindow:showFlat" << std::endl;
	viewer->setDrawMode(DRAW_MODE::FLAT);
}

void MainWindow::showSmooth()
{
	std::cout << "MainWindow:showSmooth" << std::endl;
	viewer->setDrawMode(DRAW_MODE::SMOOTH);
}

void MainWindow::setModePoints()
{
	viewPoints->setChecked(true);
}

void MainWindow::setModeWireframe()
{
	viewWireframe->setChecked(true);
}
