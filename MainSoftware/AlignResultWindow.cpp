#include "AlignResultWindow.h"

AlignResultWindow::AlignResultWindow()
{
	viewer = new MultipleMeshViewer();
	initResultWindow();
}

// overload constructor
AlignResultWindow::AlignResultWindow(PlyCloud * mesh)
{
	viewer = new MultipleMeshViewer();
	viewer->acceptMesh(mesh);
	initResultWindow();
}

// overload constructor
AlignResultWindow::AlignResultWindow(PlyCloud * mesh1, PlyCloud * mesh2)
{
	viewer = new MultipleMeshViewer();
	viewer->addMesh(mesh1);
	viewer->addMesh(mesh2);
	initResultWindow();
}

AlignResultWindow::~AlignResultWindow()
{
}

void AlignResultWindow::initResultWindow()
{
	this->setGeometry(300, 300, 1800, 1350);
	
	// init actions
	save = new QAction(tr("&Save"), this);
	save->setIcon(QIcon(":/icons/images/save.png"));
	save->setStatusTip(tr("Save the aligned mesh"));
	connect(save, SIGNAL(triggered()), viewer, SLOT(saveMesh()));

	lightControl = new checkableAction(this);
	lightControl->setText(tr("Control Light"));
	lightControl->setIcon(QIcon(":/icons/images/light.png"));
	lightControl->setStatusTip(tr("Turn On or Off Light"));
	lightControl->setCheckable(true);
	lightControl->setChecked(true);
	connect(lightControl, SIGNAL(actionCheck()), viewer, SLOT(turnOnLight()));
	connect(lightControl, SIGNAL(actionUncheck()), viewer, SLOT(turnOffLight()));

	merge = new QAction(tr("&Merge"), this);
	merge->setText(tr("Merge Aligned Meshes"));
	merge->setIcon(QIcon(":/icons/images/merge.png"));
	merge->setStatusTip(tr("Merge Aligned Meshes"));
	connect(merge, SIGNAL(triggered()), this, SLOT(mergeMeshes()));

	split = new QAction(tr("S&plit"), this);
	split->setText(tr("Return to Split Meshes"));
	split->setIcon(QIcon(":/icons/images/split.png"));
	split->setStatusTip(tr("Return to Split Meshes"));
	connect(split, SIGNAL(triggered()), this, SLOT(showSplitMeshes()));

	toolBar = new QToolBar();
	toolBar->addAction(save);
	toolBar->addAction(lightControl);
	toolBar->addAction(merge);
	toolBar->addAction(split);

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->addWidget(toolBar);
	vLayout->addWidget(viewer);
	this->setLayout(vLayout);
}

void AlignResultWindow::mergeMeshes()
{
	viewer->mergeMeshes();
}

void AlignResultWindow::showSplitMeshes()
{
	viewer->viewSplitMeshes();
}