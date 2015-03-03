// This is a window to show the align result and save it

#pragma once
#include <QWidget>
#include <QVBoxLayout>
#include <QToolBar>
#include <QAction>

#include "MultipleMeshViewer.h"
#include "checkableAction.h"


/*!
* \brief Used to show the alignment result and then save the aligned & merged mesh to file.
* \details This window can show one or two mesh. If one mesh is passed to constructor, it should be merged mesh after alignment. If two meshes are passed, they should be aligned already.
* \author Jerome Jiang
*/
class AlignResultWindow :
	public QWidget
{
	Q_OBJECT
public:

	/// Constructor
	AlignResultWindow();

	/// overload constructor
	/// \param mesh to be shown in this window
	AlignResultWindow(PlyCloud *mesh);

	/// overload constructor
	/// \param mesh1 the first mesh that to be shown
	/// \param mesh2 the second mesh that to be shown
	/// They should be aligned already
	AlignResultWindow(PlyCloud * mesh1, PlyCloud * mesh2);

	/// Deconstructor
	~AlignResultWindow();

private:

	/// Initialize the window
	void initResultWindow();

private slots:

	/// Slots: To merge the two meshes, if two meshes are passed into the class
	void mergeMeshes();

	/// Slots: If two meshes are passed into the class, we can show the merged mesh. Or if user would, we can also show the two meshes separately.
	void showSplitMeshes();

private:

	/// Mesh Viewer in this window
	MultipleMeshViewer * viewer;

	/// Toolbar
	QToolBar * toolBar;

	/// Button for saving the mesh
	QAction * save;

	/// Button for merge two meshes
	QAction * merge;

	/// Button for show two meshes separately
	QAction * split;

	/// Light control for viewer
	checkableAction * lightControl;
};

