// This is a window to show the align result and save it

#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>
#include "meshviewer.h"

class AlignResultWindow :
	public QWidget
{
	Q_OBJECT
public:
	AlignResultWindow();

	// overload constructor
	AlignResultWindow(PlyCloud *);
	~AlignResultWindow();

private:
	void initResultWindow();

private:
	MeshViewer * viewer;
	QToolBar * toolBar;
	QAction * save;
};

