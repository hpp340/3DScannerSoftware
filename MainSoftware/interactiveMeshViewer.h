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
	interactiveMeshViewer();
	~interactiveMeshViewer();

public slots:
	void enterSelectionMode();
	void quitSelectionMode();

protected:
	// override 
	void mousePressEvent(QMouseEvent *);
	void drawMesh();

private:
	bool isSelectionMode;
	std::vector<int> allSelectedVertex;
private:
	int selectVertex();
	void drawSelectedVertex(int vertexId);
};

