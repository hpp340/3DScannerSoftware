#include "mainwindow.h"
#include "alignWindow.h"
#include "PoissonRecon.h"
#include "SurfaceTrim.h"
#include "sensorWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
	viewer = new FullFuncMeshViewer();
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

	reconAction = new QAction(tr("&Reconstruction"), this);
	reconAction->setIcon(QIcon(":/icons/images/recon.png"));
	reconAction->setText(tr("Poisson Surface Reconstructioin"));
	reconAction->setStatusTip(tr("Poisson Surface Reconstruction"));
	connect(reconAction, SIGNAL(triggered()), this, SLOT(startPoissonRecon()));

	trimAction = new QAction(tr("Trim Mesh"), this);
	trimAction->setIcon(QIcon(":/icons/images/trim.png"));
	trimAction->setText(tr("Trim Mesh"));
	trimAction->setStatusTip(tr("Trim Mesh"));
	connect(trimAction, SIGNAL(triggered()), this, SLOT(startTrim()));

	selectAction = new checkableAction(this);
	selectAction->setIcon(QIcon(":/icons/images/selectgroup.png"));
	selectAction->setText(tr("Select a Group of Points"));
	selectAction->setStatusTip(tr("Select a Group of Points"));
	selectAction->setCheckable(true);
	selectAction->setChecked(false);
	connect(selectAction, SIGNAL(actionCheck()), viewer, SLOT(enterSelectionMode()));
	connect(selectAction, SIGNAL(actionUncheck()), viewer, SLOT(quitSelectionMode()));

	clearAction = new QAction(tr("Clear Selected"), this);
	clearAction->setIcon(QIcon(":/icons/images/clear.png"));
	clearAction->setStatusTip(tr("Clear Selected Points"));
	connect(clearAction, SIGNAL(triggered()), viewer, SLOT(clearSelected()));

	deleteAction = new QAction(tr("Delete Selected Points"), this);
	deleteAction->setIcon(QIcon(":/icons/images/delete.png"));
	deleteAction->setStatusTip(tr("Delete Selected Points and Faces"));
	connect(deleteAction, SIGNAL(triggered()), viewer, SLOT(deleteSelected()));

	sensorStart = new QAction(tr("Start PrimeSense Sensor"), this);
	sensorStart->setIcon(QIcon(":/icons/images/sensor.png"));
	sensorStart->setStatusTip(tr("Start PrimeSense Sensor"));
	connect(sensorStart, SIGNAL(triggered()), this, SLOT(startSensor()));

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

	showTexture = new checkableAction(this);
	showTexture->setIcon(QIcon(":/icons/images/texture.png"));
	showTexture->setText(tr("Show/Hide Texture"));
	showTexture->setStatusTip(tr("Show or Hide Texture"));
	showTexture->setCheckable(true);
	showTexture->setChecked(false);
	connect(showTexture, SIGNAL(actionCheck()), viewer, SLOT(textureOn()));
	connect(showTexture, SIGNAL(actionUncheck()), viewer, SLOT(textureOff()));
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
	editToolbar->addAction(trimAction);

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
	viewToolbar->addAction(showTexture);

	deleteToolbar = addToolBar(tr("&Delete"));
	deleteToolbar->addAction(selectAction);
	deleteToolbar->addAction(clearAction);
	deleteToolbar->addAction(deleteAction);

	extraToolbar = addToolBar(tr("E&xtra Tools"));
	extraToolbar->addAction(sensorStart);
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
	surfaceRecon->startRecon< 2, PlyValueVertex< Real >, true >();
	//viewer->acceptMesh(surfaceRecon->getSurface());
	viewer->loadFile("reconOutput.ply", "ply");
}

void MainWindow::startTrim()
{
	std::cout << "MainWindow:startTrim.." << std::endl;
	SurfaceTrim * trimmer = new SurfaceTrim();
	bool isTrimOK = trimmer->startSurfaceTrim("reconOutput.ply");
	if (isTrimOK)
	{
		viewer->loadFile("reconOutputTrimmed.ply", "ply");
	}
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

void MainWindow::startSensor()
{
	sensorWindow * showSensorWindow = new sensorWindow();
	showSensorWindow->show();
}