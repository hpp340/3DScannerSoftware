#pragma once
#include "meshviewer.h"

#ifndef SELECT_MIN_DIST
#define SELECT_MIN_DIST 0.01
#endif

class FullFuncMeshViewer :
	public MeshViewer
{
	Q_OBJECT
public:
	FullFuncMeshViewer();
	~FullFuncMeshViewer();

protected:
	// override
	// void mouseReleaseEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	// draw point
	void drawMeshPoints();
	//// draw wireframe
	//void drawMeshWireframe();
	//// draw flat lines
	//void drawMeshFlatlines();
	//// draw flat
	//void drawMeshFlat();
	//// draw smooth
	//void drawMeshSmooth();

public slots:
	void enterSelectionMode();
	void quitSelectionMode();
	// clear the vector of selected points
	void clearSelected();
	// delete the selected points and faces
	void deleteSelected();

private:
	void selectGroupVertex(QPoint);
	glm::dvec3 getRayVector(QPoint, glm::dvec3 &nearPt, glm::dvec3 &farPt);

private:
	bool isSelectionMode;
	std::vector<int> selectedVertices;
};

