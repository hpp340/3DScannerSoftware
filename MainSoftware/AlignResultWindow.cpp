#include "AlignResultWindow.h"

AlignResultWindow::AlignResultWindow()
{
	viewer = new MeshViewer();
	initResultWindow();
}

// overload constructor
AlignResultWindow::AlignResultWindow(PlyCloud * mesh)
{
	viewer = new MeshViewer();
	viewer->acceptMesh(mesh);
	initResultWindow();
}

AlignResultWindow::~AlignResultWindow()
{
}

void AlignResultWindow::initResultWindow()
{
	this->setGeometry(300, 300, 1000, 800);
	
	save = new QAction(tr("&Save"), this);
	save->setIcon(QIcon(":/icons/images/save.png"));
	save->setStatusTip(tr("Save the aligned mesh"));
	connect(save, SIGNAL(triggered()), viewer, SLOT(saveMesh()));

	toolBar = new QToolBar();
	toolBar->addAction(save);

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->addWidget(toolBar);
	vLayout->addWidget(viewer);
	this->setLayout(vLayout);
}
