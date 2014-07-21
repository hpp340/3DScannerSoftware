#pragma once
#include <QWidget>
#include <QToolbar>
#include <QtGui>
#include <QAction>
#include <QGridLayout>
#include "meshviewer.h"

class alignWindow :
	public QWidget
{
	Q_OBJECT
public:
	alignWindow();
	~alignWindow();

private:
	void initAlignWindow();
	void createActions();
	void createToolbar();

private:
	QAction * openFirstMesh;
	QAction * openSecondMesh;
	QAction * alignButton;
	QAction * enterSelection;

	QToolBar * alignToolbar;
	MeshViewer * firstMesh;
	MeshViewer * secondMesh;
};

