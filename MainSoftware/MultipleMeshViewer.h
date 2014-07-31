#pragma once
#include "meshviewer.h"
#include "header\ANN\ANN.h"

class MultipleMeshViewer :
	public MeshViewer
{
	Q_OBJECT
public:
	MultipleMeshViewer();
	~MultipleMeshViewer();
	// add mesh to meshList
	void addMesh(PlyCloud *);
	void mergeMeshes();
	// override
	void saveFile(const char *);
private:
	void initColorVector();
	// override
	void paintGL();
	void drawMesh();
	void drawMergedMesh();
	void drawMergedMeshOK();
private:
	std::vector<PlyCloud *> meshList;
	glm::vec3 colorVector;
	std::vector<glm::vec3> colorList;
	PlyCloud * mergedMesh;
	ANNkd_tree * kdTree;
	bool showMergedMesh;
	bool isMeshesMerged;
public slots:
	void saveMesh();
};

