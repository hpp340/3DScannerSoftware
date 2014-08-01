/*
 * Used as the class of point cloud
 * Author: Jerome Jiang
 * Date: 6/27/2014
 * Updated: 7/23/2014
*/

#pragma once

// #include "stdafx.h"
#include "Point.h"
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "../JFace.h"

using namespace MeshLib;
using namespace std;

enum class VertexInfo {NONE, POSX, POSY, POSZ, NORMALX, NORMALY, NORMALZ, VALUE};

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

	// overload constructor
	PlyCloud(std::vector<CPoint> newVertexList, std::vector<JFace> newFaceList);

	// deconstructor
	~PlyCloud();

	// read in the ply file, construct the vertex list used for ICP
	// please make sure that the coordinates of vertex is the first three properties 
	bool read_ply(const char *filename);
	
	// write the point cloud to ply file
	bool write_ply(const char *fliename);

	// get the vertex whose id is v_id
	CPoint get_vertex(size_t v_id);

	// get the normal that the associated vertex id is v_id
	CPoint get_normal(size_t v_id);

	// get vertex list
	std::vector<CPoint> get_vertex_list();

	// get normal list
	std::vector<CPoint> get_normal_list();

	// get face list
	std::vector<JFace> get_face_list();

	// get deleted vertex
	std::vector<bool> get_deleted_vertex_list();
	// get vertex number
	int get_vertex_num();
	// get face number
	int get_face_num();

private:
	// add vertex to the vertex list and add normal to the normal list of the point cloud
	void add_vert(CPoint v);
	void add_norm(CPoint n);
	void add_value(double value);
	void add_face(JFace f);
	void add_face_normal(CPoint);
	// setup vertex property
	void setupVertexProperty(string inS, int propIter);
	// compute face normal
	void computeFaceNormal();
	// compute vertex normal
	void computeVertexNormal();
	std::vector<int> findFacesVertex(int _vert);
	// compute cross product
	CPoint crossProduct(CPoint p1, CPoint p2);
	// normalize mesh
	void normalizeMesh();
	// clean mesh
	void cleanMesh();

protected:
	// vertex property list
	std::vector<VertexInfo> vertProperty;
	// vertex number
	int vertex_num;
	// face number
	int face_num;
	// deleted vertex list
	std::vector<bool> deleted_vertex_list;
	// vertex list
	std::vector<CPoint> vertex_list;
	// normal list
	std::vector<CPoint> normal_list;
	// face list
	std::vector<JFace> face_list;
	// face normal list
	std::vector<CPoint> face_normal_list;
	// value list
	std::vector<double> value_list;
	// bool
	bool hasVertexPos, hasNormal, hasValue, hasFace;
};

