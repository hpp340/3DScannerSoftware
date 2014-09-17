/*
 * Used as the class of point cloud
 * Author: Jerome Jiang
 * Date: 6/27/2014
 * Updated: 7/23/2014
 * Updated: 8/5/2014
 */

#pragma once

// #include "stdafx.h"
#include "Point.h"
#include "Point2.h"
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "../JFace.h"
#include "../JVertex.h"
#include <OpenNI.h>

using namespace MeshLib;
using namespace std;
using namespace JMesh;
enum class VertexInfo { NONE, POSX, POSY, POSZ, NORMALX, NORMALY, NORMALZ, VALUE };

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
	PlyCloud(std::vector<CPoint> newVertexList, std::vector<JFace *> newFaceList);

	// overload constructor
	PlyCloud(std::vector<CPoint> newVertexList, std::vector<openni::RGB888Pixel> colorList);

	// deconstructor
	~PlyCloud();

	// read in the ply file, construct the vertex list, face list, normal list
	bool read_ply(const char *filename);

	// read in obj file, construct the vertex list
	bool read_obj(const char *filename);

	// write the point cloud to ply file
	bool write_ply(const char *fliename);

	// write the point cloud to obj file
	bool write_obj(const char *filename);

	// get the vertex whose id is v_id
	JVertex * get_vertex(size_t v_id);

	// new version get vertex list
	std::vector<JVertex *> getJVertexList() { return JVertexList; };

	// get color list
	std::vector<openni::RGB888Pixel> getColorList() { return color_list; }

	// get face list
	std::vector<JFace*> get_face_list();

	// get deleted vertex
	std::vector<bool> get_deleted_vertex_list();
	// get deleted faces list
	std::vector<bool> get_deleted_face_list();
	// add deleted vertex by id
	void add_deleted_vertex(int);
	// add deleted face by id
	void add_deleted_face(int);
	// get vertex number
	int get_vertex_num();
	// get face number
	int get_face_num();

private:
	// add vertex to the vertex list and add normal to the normal list of the point cloud

	void addJVert(JVertex * vert);
	void add_norm(CPoint n);
	void add_value(double value);
	void add_face(JFace *f);

	// setup vertex property
	void setupVertexProperty(string inS, int propIter);
	// compute face normal
	void computeFaceNormal();
	// compute vertex normal
	void computeVertexNormal();

	// compute cross product
	CPoint crossProduct(CPoint p1, CPoint p2);
	// normalize mesh
	void normalizeMesh();
	// clean mesh - delete those vertices who dont have normal
	void cleanMesh();

public:
	bool hasVertexPos() { return existVertexPos; };
	bool hasNormal() { return existNormal; };
	bool hasValue() { return existValue; };
	bool hasFace() { return existFace; };
	bool hasTexture() { return existTexture; };
	bool hasColor() { return existColor; }

protected:
	// vertex property list
	std::vector<VertexInfo> vertProperty;
	// vertex number
	int vertex_num;
	// face number
	int face_num;
	std::vector<int> newVertexIdList;
	// deleted vertex list
	std::vector<bool> deleted_vertex_list;
	// deleted face list
	std::vector<bool> deleted_face_list;
	// texture coordinates list
	std::vector<CPoint2> texture_list;
	// normal list
	std::vector<CPoint> normal_list;
	// face list
	std::vector<JFace *> face_list;
	// face normal list
	std::vector<CPoint> face_normal_list;
	// value list
	std::vector<double> value_list;

	// new version vertex list
	std::vector<JVertex *> JVertexList;
	// color list
	std::vector<openni::RGB888Pixel> color_list;

	// bool
	bool existVertexPos, existNormal, existValue, existFace, existTexture, existColor;
};

