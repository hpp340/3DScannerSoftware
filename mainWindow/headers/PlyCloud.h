/*
 * Used as the class of point cloud
 * Author: Jerome Jiang
 * Date: 6/27/2014
*/

#pragma once

#include "Point.h"
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>

using namespace MeshLib;
using namespace std;
class PlyCloud
{
public:
	// constructor
	PlyCloud();

	// overload constructor
	// construct the class with a list of vertex and a list of normal
	PlyCloud(std::vector<CPoint> newVertexList, vector<CPoint> newNormalList);

	// overload constructor
	// construct the class with a list of vertex
	PlyCloud(std::vector<CPoint> newVertexList);

	// deconstructor
	~PlyCloud();

	// read in the ply file, construct the vertex list used for ICP
	// please make sure that the coordinates of vertex is the first three properties 
	bool read_ply(const char *filename);
	
	// write the point cloud to ply file
	int write_ply(const char *fliename);

	// get the vertex whose id is v_id
	CPoint get_vertex(size_t v_id);

	// get the normal that the associated vertex id is v_id
	CPoint get_normal(size_t v_id);

	// get vertex list
	std::vector<CPoint> get_vertex_list();

	// get normal list
	std::vector<CPoint> get_normal_list();

private:
	// add vertex to the vertex list and add normal to the normal list of the point cloud
	int add_VertNorm(CPoint v, CPoint n);

protected:
	// vertex list
	std::vector<CPoint> vertex_list;
	std::vector<CPoint> normal_list;
};

