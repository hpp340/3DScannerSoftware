/*
 * Used as the class of point cloud
 * Author: Jerome Jiang
 * Date: 6/27/2014
 * Updated: 7/23/2014
 * Updated: 8/5/2014
 */

#pragma once

/// #include "stdafx.h"
#include "Point.h"
#include "Point2.h"
#include <stdlib.h>
#include <vector>
#include <fstream>
#include <iostream>

#include "JFace.h"
#include "JVertex.h"

using namespace MeshLib;
using namespace std;
using namespace JMesh;
enum class VertexInfo { NONE, POSX, POSY, POSZ, NORMALX, NORMALY, NORMALZ, VALUE, COLORRED, COLORGREEN, COLORBLUE };


/*! 
* \brief Data Structure mainly for point cloud, part of triangle mesh.
* \details HalfEdge datastructure is not implemented. Supported file format: *.ply and *.obj
* \details All lists related to vertex should be of the same size of vertex list. And all lists related to face shoule be of the same size of face list.
*/
class PlyCloud
{
public:

	/// constructor
	PlyCloud();

	/// overload constructor
	/// Construct the class with a list of vertex and a list of normal
	/// \param newVertexList vertex list
	/// \param newNormalList normal list
	PlyCloud(std::vector<CPoint> newVertexList, vector<CPoint> newNormalList);

	/// overload constructor
	/// Construct the class with a list of vertex
	/// \param newVertexList vertex list
	PlyCloud(std::vector<CPoint> newVertexList);

	/// overload constructor
	/// Construct the class with a list of vertices and a list of faces
	/// \param newVertexList vertex list
	/// \param newFaceList face list
	PlyCloud(std::vector<CPoint> newVertexList, std::vector<JFace *> newFaceList);

	/// overload constructor
	/// Construct the class with a list of vertices and the their colors
	/// \param newVertexList vertex list
	/// \param colorList color list. 
	/// They should be of the same size
	PlyCloud(std::vector<CPoint> newVertexList, std::vector<JColor> colorList);

	/// Deconstructor
	~PlyCloud();

	/// Read in the ply file, construct the vertex list, face list, normal list
	/// \param filename ply file name
	bool read_ply(const char *filename);

	/// read in obj file, construct the vertex list
	/// \param filename obj file name
	bool read_obj(const char *filename);

	/// write the point cloud to ply file
	bool write_ply(const char *fliename);

	/// write the point cloud to obj file
	bool write_obj(const char *filename);

	/// get the vertex whose id is v_id
	/// \param v_id vertex id. The index in the vertex list
	JVertex * get_vertex(size_t v_id);

	/// Get the Vertex List contains pointer to JVertex.
	/// \return JVertexList the list containing pointers to all vertices
	std::vector<JVertex *> getJVertexList() { return JVertexList; };

	/// get color list
	std::vector<JColor> getColorList() { return color_list; }

	/// get face list
	std::vector<JFace*> get_face_list();

	/// get deleted vertex
	std::vector<bool> get_deleted_vertex_list();

	/// get deleted faces list
	std::vector<bool> get_deleted_face_list();

	/// add deleted vertex by id
	void add_deleted_vertex(int);

	/// add deleted face by id
	void add_deleted_face(int);

	/// get vertex number
	int get_vertex_num();

	/// get face number
	int get_face_num();

	/// normalize
	void normalize() { normalizeMesh(); };

private:

	/// add vertex to the vertex list
	/// \param vert pointer to the vertex to be added
	void addJVert(JVertex * vert);

	/// add normal to the normal list of the point cloud
	void add_norm(CPoint n);

	/// add value to the value list of the point cloud
	void add_value(double value);

	/// add face to the face list of the point cloud
	void add_face(JFace *f);

	/// setup vertex property
	/// - inS: current line of string
	/// - propIter: specifies which property
	void setupVertexProperty(string inS, int propIter);

	/// compute face normal
	void computeFaceNormal();

	/// compute vertex normal
	void computeVertexNormal();

	/// compute cross product p1 X p2
	/// \param p1 one of the vector
	/// \param p2 the other vector
	CPoint crossProduct(CPoint p1, CPoint p2);

	/// normalize mesh or the mesh will be too big to fit in the scene
	void normalizeMesh();

	/// clean mesh - delete those vertices who dont have normal
	void cleanMesh();

public:
	bool hasVertexPos() { return existVertexPos; };
	bool hasNormal() { return existNormal; };
	bool hasValue() { return existValue; };
	bool hasFace() { return existFace; };
	bool hasTexture() { return existTexture; };
	bool hasColor() { return existColor; }

protected:

	/// vertex property list
	std::vector<VertexInfo> vertProperty;

	/// vertex number
	int vertex_num;

	/// face number
	int face_num;

	/// vertex list
	std::vector<int> newVertexIdList;

	/// deleted vertex list
	std::vector<bool> deleted_vertex_list;

	/// deleted face list
	std::vector<bool> deleted_face_list;

	/// texture coordinates list
	std::vector<CPoint2> texture_list;

	/// normal list
	std::vector<CPoint> normal_list;

	/// face list
	std::vector<JFace *> face_list;

	/// face normal list
	std::vector<CPoint> face_normal_list;

	/// value list
	std::vector<double> value_list;

	/// new version vertex list
	std::vector<JVertex *> JVertexList;

	/// color list
	std::vector<JColor> color_list;

	/// bool flags to check if vertex position, normal, value, face, texture and color exist in the mesh file
	bool existVertexPos, existNormal, existValue, existFace, existTexture, existColor;
};

