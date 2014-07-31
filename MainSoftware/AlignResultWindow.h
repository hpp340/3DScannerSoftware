// This is a window to show the align result and save it

#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>

#include "MultipleMeshViewer.h"
#include "checkableAction.h"

class AlignResultWindow :
	public QWidget
{
	Q_OBJECT
public:
	AlignResultWindow();

	// overload constructor
	AlignResultWindow(PlyCloud *);
	// overload constructor
	AlignResultWindow(PlyCloud *, PlyCloud *);
	~AlignResultWindow();

private:
	void initResultWindow();

private slots:
	void mergeMeshes();
	void showSplitMeshes();

private:
	MultipleMeshViewer * viewer;
	QToolBar * toolBar;
	QAction * save;
	QAction * merge;
	QAction * split;
	checkableAction * lightControl;
};

