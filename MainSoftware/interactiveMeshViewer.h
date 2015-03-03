#pragma once
#include "meshviewer.h"

/*! \brief This class is used to interactive with user input such as selection.
* \details New features compared with parent class MeshViewer: response to mouse press - if enter selection mode.
* \details Used in alignWindow to align two meshes manually.
* \author Jerome Jiang
*/

class interactiveMeshViewer :
	public MeshViewer
{
	Q_OBJECT
public:

	/// Constructor.
	/// Selection mode is set to be false by default.
	interactiveMeshViewer();

	/// Overload constructor.
	/// Selection mode is set to be false by default.
	/// \param numMesh ID of mesh
	interactiveMeshViewer(int numMesh);

	/// Deconstructor
	~interactiveMeshViewer();

	/// Get selected vertex vector
	/// \return the list of selected vertices
	std::vector<int> getSelectedVertex() { return allSelectedVertex; }

public slots:

	/// Enter the selection mode. flag is set to be true.
	void enterSelectionMode();

	/// Quit the selection mode. flag is set to be false.
	void quitSelectionMode();

protected:

	/// Override.
	/// Response to mouse press event.
	void mousePressEvent(QMouseEvent *);
	
	/// Draw mesh
	virtual void drawMesh();

private:

	/// Response to mouse press event. Find the closest vertex near the click position.
	/// \return the ID of the closest vertex
	int selectVertex();

	/// Selected vertices must be drawn in different color, which is completed by this method.
	/// \param vertexId the vertex to be drawn as selected 
	void drawSelectedVertex(int vertexId);

private:

	/// flag indicating if the selection mode is entered.
	bool isSelectionMode;

	/// ID of the mesh passed into the class.
	int meshID;

protected:

	/// List of all selected vertices
	std::vector<int> allSelectedVertex;

};

