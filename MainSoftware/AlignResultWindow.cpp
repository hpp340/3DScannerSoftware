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
	this->setGeometry(300, 300, 1000, 800);
	
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

	toolBar = new QToolBar();
	toolBar->addAction(save);
	toolBar->addAction(lightControl);

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->addWidget(toolBar);
	vLayout->addWidget(viewer);
	this->setLayout(vLayout);
}