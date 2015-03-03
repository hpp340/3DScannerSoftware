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

/*! \brief This class is used as the registration window where manual alignment of two meshes is finished.
* \details Two mesh viewers are shown in this winodw to finish the alignment task. Each mesh in each viewer must be able to be selected. To finish this, we have a mode called selection mode and a related button.
* \details If that button is checked, we enter the selection mode, any click in the viewer scene will result a selected vertex(search nearest vertex).
* \details All selection in the two viewers must be in consistent order. At least four pairs of corresponding vertices must be selected to compute the rotation and translation.
* \author: Jerome Jiang
* \date: 7/22/2014
*/
class alignWindow :
	public QWidget
{
	Q_OBJECT
public:

	/// Construtor
	alignWindow();

	/// Deconstructor
	~alignWindow();

private:

	/// Initialize the align window
	void initAlignWindow();

	/// Create all actions(buttons) needed
	void createActions();

	/// Create toolbar
	void createToolbar();
	
private slots:

	/// Once the button of selection is checked, this slot is triggerred
	void enterSelectionMode();

	/// Once the button of selection is unchecked, this slot is triggered
	void quitSelectionMode();

	/// Slots starting to do alignment/registration. At least four pairs of vertices must be selected.
	void alignMeshes();
	
private:

	/// Compute the transformation matrix
	void computeTransformation();

	/// Apply the transformation matrix to the first mesh such that two meshes are aligned together.
	void transformMesh();

	/// Show result window. The AlignResutlWindow will pop up and show the result.
	void showResultWindow();

private:

	/// Action. Open first mesh.
	QAction * openFirstMesh;

	/// Action. Open second mesh.
	QAction * openSecondMesh;

	/// Action. Start to align.
	QAction * alignButton;

	/// Action. Enter selection mode.
	checkableAction * enterSelection;

	/// Action. Light control for both of the viewers.
	checkableAction * lightControl;

	/// Toolbar
	QToolBar * alignToolbar;

	/// Two meshes to be aligned. Shown in two viewers.
	interactiveMeshViewer * firstMeshViewer;
	interactiveMeshViewer * secondMeshViewer;

	/// Flag of selection mode
	bool selectionMode;

	/// Transformation matrix
	Matrix<double, 4, 4> transformationMatrix;

	/// Transformed mesh
	PlyCloud * newMesh;

	/// Show result window
	AlignResultWindow * resultWindow;

	/// flag indicating if transformation has been computed
	bool isComputationOK;
};

