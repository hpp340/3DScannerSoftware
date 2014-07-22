#include "alignWindow.h"

alignWindow::alignWindow()
{
	initAlignWindow();
	selectionMode = false;
}

alignWindow::~alignWindow()
{
}

void alignWindow::initAlignWindow()
{
	this->setGeometry(100, 100, 1000, 800);
	firstMesh = new interactiveMeshViewer();
	firstMesh->loadFile("genus2ply.ply");
	secondMesh = new interactiveMeshViewer();
	secondMesh->loadFile("genus2ply.ply");
	createActions();
	createToolbar();
	QGridLayout * alignLayout = new QGridLayout();
	alignLayout->addWidget(alignToolbar, 0, 0, 1, -1);
	alignLayout->addWidget(firstMesh, 1, 0, 1, 1);
	alignLayout->addWidget(secondMesh, 1, 1, 1, 1);
	this->setLayout(alignLayout);

}

void alignWindow::createActions()
{
	openFirstMesh = new QAction(tr("Open &First Mesh"), this);
	openFirstMesh->setIcon(QIcon(":/icons/images/open.ico"));
	openFirstMesh->setStatusTip(tr("Open First Mesh"));
	// todo: connect

	openSecondMesh = new QAction(tr("Open &Second Mesh"), this);
	openSecondMesh->setIcon(QIcon(":/icons/images/open.ico"));
	openSecondMesh->setStatusTip(tr("Open Second Mesh"));
	// todo: connect

	alignButton = new QAction(tr("&Align"), this);
	alignButton->setIcon(QIcon(":/icons/images/align.png"));
	alignButton->setStatusTip(tr("Align Two Meshes"));
	alignButton->setShortcut(tr("Ctrl+l"));
	// todo: connect

	enterSelection = new checkableAction(this);
	enterSelection->setText(tr("&Select"));
	enterSelection->setIcon(QIcon(":/icons/images/select.png"));
	enterSelection->setStatusTip(tr("Enter Selection Mode"));
	enterSelection->setCheckable(true);
	enterSelection->setChecked(false);
	// enter selection mode
	connect(enterSelection, SIGNAL(actionCheck()), this, SLOT(enterSelectionMode()));
	connect(enterSelection, SIGNAL(actionCheck()), firstMesh, SLOT(enterSelectionMode()));
	connect(enterSelection, SIGNAL(actionCheck()), secondMesh, SLOT(enterSelectionMode()));
	// quit selection mode
	connect(enterSelection, SIGNAL(actionUncheck()), this, SLOT(quitSelectionMode()));
	connect(enterSelection, SIGNAL(actionUncheck()), firstMesh, SLOT(quitSelectionMode()));
	connect(enterSelection, SIGNAL(actionUncheck()), secondMesh, SLOT(quitSelectionMode()));
	// todo: connect
}

void alignWindow::createToolbar()
{
	alignToolbar = new QToolBar();
	alignToolbar->addAction(openFirstMesh);
	alignToolbar->addAction(openSecondMesh);
	alignToolbar->addAction(alignButton);
	alignToolbar->addAction(enterSelection);
}

void alignWindow::enterSelectionMode()
{
	if ((!selectionMode) && (enterSelection->isChecked()))
	{
		std::cout << "alignwindow:enter selection mode" << std::endl;
		selectionMode = true;
	}
}

void alignWindow::quitSelectionMode()
{
	if (selectionMode)
	{
		std::cout << "quit selection mode" << std::endl;
		selectionMode = false;
	}
}