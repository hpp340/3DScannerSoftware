#pragma once
#include "meshviewer.h"

#ifndef SELECT_MIN_DIST
#define SELECT_MIN_DIST 0.01
#endif

/*!
* \brief Mesh viewer with all the functions including selection/deletion.
* \details Added selection & deletion points and faces function. And this class is used in the main viewer.
* \author Jerome Jiang
* \date 8/6/2014
*/
class FullFuncMeshViewer :
	public MeshViewer
{
	Q_OBJECT
public:
	///  Constructor
	FullFuncMeshViewer();

	///  Deconstructor
	~FullFuncMeshViewer();

protected:
	
	///  void mouseReleaseEvent(QMouseEvent *);
	///  Override.
	/// Response to mouse moving event.
	void mouseMoveEvent(QMouseEvent *);

	///  Draw point cloud
	void drawMeshPoints();

	///  Draw wireframe
	void drawMeshWireframe();

	///  Draw flat shading
	void drawMeshFlat();

	///  Draw smooth shading
	void drawMeshSmooth();

public slots:

	/// Slots used to enter selection mode. flag set to be true.
	void enterSelectionMode();

	/// Slots used to quit selection mode. flag set to be false.
	void quitSelectionMode();

	///  clear the vector of selected points i.e. no vertex is selected
	void clearSelected();

	///  delete the selected points and faces
	void deleteSelected();

private:

	/// Select a group of vertice which are near the cursor and along the cursor trace 
	void selectGroupVertex(QPoint);

	/// Get the ray produced from the current position of cursor to the back plane of view space
	glm::dvec3 getRayVector(QPoint, glm::dvec3 &nearPt, glm::dvec3 &farPt);

private:

	/// flag indicating if the user has entered selection mode
	bool isSelectionMode;

	/// List of selected vertices containing vertex IDs
	std::vector<int> selectedVertices;

	/// List of selected faces containing face IDs
	std::vector<int> selectedFaces;
};

