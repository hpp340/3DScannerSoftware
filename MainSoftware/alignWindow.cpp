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
	this->setGeometry(100, 100, 1400, 1000);
	firstMeshViewer = new interactiveMeshViewer();
	//firstMesh->loadFile("testx.ply");
	secondMeshViewer = new interactiveMeshViewer();
	//secondMesh->loadFile("testx.ply");
	createActions();
	createToolbar();
	QGridLayout * alignLayout = new QGridLayout();
	alignLayout->addWidget(alignToolbar, 0, 0, 1, -1);
	alignLayout->addWidget(firstMeshViewer, 1, 0, 1, 1);
	alignLayout->addWidget(secondMeshViewer, 1, 1, 1, 1);
	this->setLayout(alignLayout);
}

void alignWindow::createActions()
{
	openFirstMesh = new QAction(tr("Open &First Mesh"), this);
	openFirstMesh->setIcon(QIcon(":/icons/images/open1.png"));
	openFirstMesh->setStatusTip(tr("Open First Mesh"));
	connect(openFirstMesh, SIGNAL(triggered()), firstMeshViewer, SLOT(openMesh()));

	openSecondMesh = new QAction(tr("Open &Second Mesh"), this);
	openSecondMesh->setIcon(QIcon(":/icons/images/open2.png"));
	openSecondMesh->setStatusTip(tr("Open Second Mesh"));
	connect(openSecondMesh, SIGNAL(triggered()), secondMeshViewer, SLOT(openMesh()));

	alignButton = new QAction(tr("&Align"), this);
	alignButton->setIcon(QIcon(":/icons/images/align.png"));
	alignButton->setStatusTip(tr("Align Two Meshes"));
	alignButton->setShortcut(tr("Ctrl+l"));
	connect(alignButton, SIGNAL(triggered()), this, SLOT(alignMeshes()));
	// todo: connect

	enterSelection = new checkableAction(this);
	enterSelection->setText(tr("&Select"));
	enterSelection->setIcon(QIcon(":/icons/images/select.png"));
	enterSelection->setStatusTip(tr("Enter Selection Mode"));
	enterSelection->setCheckable(true);
	enterSelection->setChecked(false);
	// enter selection mode
	connect(enterSelection, SIGNAL(actionCheck()), this, SLOT(enterSelectionMode()));
	connect(enterSelection, SIGNAL(actionCheck()), firstMeshViewer, SLOT(enterSelectionMode()));
	connect(enterSelection, SIGNAL(actionCheck()), secondMeshViewer, SLOT(enterSelectionMode()));
	// quit selection mode
	connect(enterSelection, SIGNAL(actionUncheck()), this, SLOT(quitSelectionMode()));
	connect(enterSelection, SIGNAL(actionUncheck()), firstMeshViewer, SLOT(quitSelectionMode()));
	connect(enterSelection, SIGNAL(actionUncheck()), secondMeshViewer, SLOT(quitSelectionMode()));
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

// slots
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

void alignWindow::alignMeshes()
{
	std::vector<int> firstSelectedVertex = firstMeshViewer->getSelectedVertex();
	std::vector<int> secondSelectedVertex = secondMeshViewer->getSelectedVertex();
	PlyCloud * firstMesh = firstMeshViewer->getMesh();
	PlyCloud * secondMesh = secondMeshViewer->getMesh();
	if ((firstSelectedVertex.size() >= 4) && (secondSelectedVertex.size() >= 4))
	{
		// matrix calculate stuff
		// solve equation Ax = b
		// where A and b contains four pairs of vertices and x is the rotation&translation matrix
		Matrix<double, 12, 12> A;
		A.setZero();
		VectorXd x(12);
		VectorXd b(12);
		x.setZero(); b.setZero();
		for (int i = 0; i < 12; i++)
		{
			int jStart = 4 * (i % 3);
			int jEnd = jStart + 4;
			int _firstVertexId = firstSelectedVertex[i / 3];
			CPoint _firstVertex = firstMesh->get_vertex(_firstVertexId);
			for (int j = jStart; j < jEnd; j++)
			{
				if (j < jEnd - 1)
				{
					A(i, j) = _firstVertex[j % 4];
				}
				else
				{
					A(i, j) = 1.0;
				}
			}
			int _secondVertexId = secondSelectedVertex[i / 3];
			CPoint _secondVertex = secondMesh->get_vertex(_secondVertexId);
			b(i) = _secondVertex[i % 3];
		}
		// solve for x
		x = A.fullPivLu().solve(b);
		std::cout << "solution:\n" << x << std::endl;
	}
	else
	{
		QMessageBox msgBoxNotEnoughVertex;
		msgBoxNotEnoughVertex.setText("Not Enough Selected Vertex.\n You must select at least 4 pairs of vertex to align.");
		msgBoxNotEnoughVertex.exec();
	}
}