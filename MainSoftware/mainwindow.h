#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtGui>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QtWidgets>

#include "meshviewer.h"
#include "FullFuncMeshViewer.h"
#include "checkableAction.h"

/*!
* \brief MainWindow is shown once the software is started. 
* \details All windows could be started from here. And all functions can be found here, all related to a button. Only one MainWindow could be created in Qt.
* \author Jerome Jiang
*/

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
	/// Constructor
    MainWindow(QWidget *parent = 0);

	/// Deconstructor
    ~MainWindow();

private:
	// show mesh with filename
	// void showMesh(const char * filename);

private slots:

	/// Slot used to show align window which is used to manually align two meshes
	void showAlignWindow();

	/// Slot used to change render mode to POINTS such that only point cloud is drawn
	void showPoints();

	/// Slot used to change render mode to WIREFRAME such that only edges are drawn
	void showWireframe();

	/// Slot used to change render mode to FLATLINES such that faces and edges are drawn in flat shading
	void showFlatlines();

	/// Slot used to change render mode to FLAT such that faces are drawn in flat shading
	void showFlat();

	/// Slot used to change render mode to FLAT such that faces are drawn in smooth shading
	void showSmooth();

private:

	/// Initialize the window such as height, width
    void initWindow();

	/// Create all actions needed for every function
    void createActions();

	/// Create menus
    void createMenus();

	/// Create toolbar
    void createToolbar();

private slots:

	/// Slot set the render mode to POINTS
	void setModePoints();

	/// Slot set the render mode to WIREFRAME
	void setModeWireframe();

	/// Start Poisson Surface Reconstruction
	void startPoissonRecon();

	/// Start trimming the reconstructed 3d model. Assume Surface Reconstruction has been finished.
	void startTrim();

	/// Start the PrimeSense sensor
	void startSensor();

private:

    /// actions - open file
    QAction * openAction;

	/// actions - save mesh to file
    QAction * saveAction;

	/// actions - save as mesh to file
    QAction * saveasAction;

	/// actions - exit the software
    QAction * exitAction;

	/// actions - undo - uncompleted
    QAction * undoAction;

	/// actions - redo - uncompleted
    QAction * redoAction;

	/// actions - show toolbar
    QAction * showToolbarAction;

	/// actions - show status bar
    QAction * showStatusAction;

	/// actions - show about the software
    QAction * showAboutAction;

	/// actions - start manually align window
	QAction * alignMeshes;

	/// actions - light control
	QAction * lightControl;

	/// edit action - Poisson Surface Recon
	QAction * reconAction;

	/// edit action - trim the surface after surface recon
	QAction * trimAction;

	/// control draw mode - points
	QAction * viewPoints;

	/// control draw mode - wireframe
	QAction * viewWireframe;

	/// control draw mode - flat line
	QAction * viewFlatlines;

	/// control draw mode - flat shading
	QAction * viewFlat;

	/// control draw mode - smooth shading
	QAction * viewSmooth;

	/// control draw mode - action group such that only one action in this group can be checked
	QActionGroup * drawModeGroup;

	/// control draw mode - texture
	checkableAction * showTexture;

	/// enter selection mode
	checkableAction * selectAction;

	/// clear selected vertices
	QAction * clearAction;

	/// delete selected vertices and faces
	QAction * deleteAction;

	/// action for starting the sensor
	QAction * sensorStart;

    // menus
    QMenu * fileMenu;
    QMenu * editMenu;
    QMenu * viewMenu;
    QMenu * helpMenu;

    // toolbars
    QToolBar * fileToolbar;
	QToolBar * editToolbar;
	QToolBar * viewToolbar;
	QToolBar * deleteToolbar;
	QToolBar * extraToolbar;

	// Main Mesh viewer. Full functional. With selection and deletion.
	FullFuncMeshViewer * viewer;
	

};

#endif // MAINWINDOW_H
