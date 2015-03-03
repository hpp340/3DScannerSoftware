#pragma once
#include "meshviewer.h"
#include "header\ANN\ANN.h"

/*!
* \brief A mesh viewer used to show more than one mesh. In different color.
* \details This class maintains a list of meshes that are added into. For each new mesh, a new color is assigned to it and it's rendered in the viewer.
* \author Jerome Jiang
*/
class MultipleMeshViewer :
	public MeshViewer
{
	Q_OBJECT
public:

	/// Constructor
	MultipleMeshViewer();
	
	/// Deconstructor
	~MultipleMeshViewer();

	/// Add new Mesh to meshList
	/// \param pointcloud The mesh that to be added
	void addMesh(PlyCloud * pointcloud);

	/// Merge meshes
	void mergeMeshes();

	/// Show meshes separately
	void viewSplitMeshes();

	/// override. Save the merged mesh.
	void saveFile(const char *);

private:

	/// Initialize the vector vector. Such that all colors are stored before meshes loading.
	void initColorVector();

	/// override to show multiple meshes
	void paintGL();

	void drawMesh();

	/// Draw merged mesh, with only one color.
	void drawMergedMesh();

	/// Control drawMergedMesh method
	void drawMergedMeshOK();

private:

	/// Mesh list containing all pointers to meshes
	std::vector<PlyCloud *> meshList;

	/// color vector, used to assign color to mesh
	glm::vec3 colorVector;

	/// color list, storing all colors
	std::vector<glm::vec3> colorList;

	/// Merged mesh
	PlyCloud * mergedMesh;

	/// Data structor for KD-Tree, used to find the nearest vertices to be regarded as neighbors
	ANNkd_tree * kdTree;

	/// flag indicating if we should show the merged mesh
	bool showMergedMesh;

	/// flag indicating if we should show the merged mesh
	bool isMeshesMerged;
public slots:

	/// Slots: save the merged mesh to file
	void saveMesh();
};

