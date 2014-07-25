/* This class is used to interactive with user input
 * inherited with MeshViewer
 * added features compared with parent class MeshViewer:
 * response to mouse press - if enter selection mode
*/

#pragma once
#include "meshviewer.h"

class interactiveMeshViewer :
	public MeshViewer
{
	Q_OBJECT
public:
	// constructor
	interactiveMeshViewer();
	// overload constructor
	interactiveMeshViewer(int numMesh);
	// deconstructor
	~interactiveMeshViewer();
	// get selected vertex vector
	std::vector<int> getSelectedVertex() { return allSelectedVertex; }

public slots:
	void enterSelectionMode();
	void quitSelectionMode();

protected:
	// override 
	void mousePressEvent(QMouseEvent *);
	
	virtual void drawMesh();

private:
	int selectVertex();
	void drawSelectedVertex(int vertexId);

private:
	bool isSelectionMode;
	int meshID;
protected:
	std::vector<int> allSelectedVertex;

};

