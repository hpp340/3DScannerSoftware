/* This class is used as the registration window where alignment of two meshes is finished
 * which is a part of our software
 * Author: Jerome Jiang
 * Date: 7/22/2014 - ?
*/
#pragma once
#include <QWidget>
#include <QToolbar>
#include <QtGui>
#include <QAction>
#include <QGridLayout>
#include <QMessageBox>
#include "header\eigen\Eigen\Dense"
#include "meshviewer.h"
#include "interactiveMeshViewer.h"
#include "checkableAction.h"
#include "AlignResultWindow.h"

using namespace Eigen;

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
	
private slots:
	void enterSelectionMode();
	void quitSelectionMode();
	// slots starting to do alignment/registration
	void alignMeshes();
	
private:
	// compute the transformation matrix
	void computeTransformation();
	// apply the transformation matrix to the first mesh
	void transformMesh();
	// show result window
	void showResultWindow();

private:
	// actions
	QAction * openFirstMesh;
	QAction * openSecondMesh;
	QAction * alignButton;
	checkableAction * enterSelection;
	checkableAction * lightControl;
	// toolbar
	QToolBar * alignToolbar;
	// two meshes to be aligned
	interactiveMeshViewer * firstMeshViewer;
	interactiveMeshViewer * secondMeshViewer;
	// flag of selection mode
	bool selectionMode;
	// transformation matrix
	Matrix<double, 4, 4> transformationMatrix;
	// transformed mesh
	PlyCloud * newMesh;
	// show result window
	AlignResultWindow * resultWindow;
	// is transformation computed
	bool isComputationOK;
};

