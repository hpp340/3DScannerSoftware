#pragma once
#include "meshviewer.h"
class MultipleMeshViewer :
	public MeshViewer
{
	Q_OBJECT
public:
	MultipleMeshViewer();
	~MultipleMeshViewer();
	// add mesh to meshList
	void addMesh(PlyCloud *);

private:
	void initColorVector();
	void drawMesh();

private:
	std::vector<PlyCloud *> meshList;
	glm::vec3 colorVector;
	std::vector<glm::vec3> colorList;
};

