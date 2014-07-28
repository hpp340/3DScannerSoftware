/* m2plyConverter.cpp : Defines the entry point for the console application.
This project is used to convert the m file to ply file used for Hoppe's Poisson Surface Reconstruction
We need vertex and its associated normal vector
Author : Jerome(Jian) Jiang
Date : 06/27/2014
Update: 7/28/2014
Update Comment: Be able to write face information into ply file. 
Noted that the vertex id starting from 1 in .m file
*/

#include "stdafx.h"

#include <iostream>
#include <fstream>
#include "BaseMesh.h"
#include "Edge.h"
#include "HalfEdge.h"
#include "Face.h"
#include "iterators.h"
#include "Vertex.h"
#include "boundary.h"
#include "ConvMesh.h"

using namespace MeshLib;
using namespace std;
int main(int argc, char** argv)
{

	if (argc < 3)
	{
		std::cout << "Not enough parameters. See useage." << std::endl;
		std::cout << "Useage : m2plyConverter.ext input.m output.ply" << std::endl;
		return 0;
	}

	bool hasFaces = false;

	ConvMesh * mesh = new ConvMesh;
	// read in the mesh data 
	mesh->read_m(argv[1]);
	cout << argv[1] << endl;
	// normalize mesh
	MeshNormalize(mesh);
	ofstream outputfile;
	outputfile.open(argv[2]);
	// write in the head of ply file
	if (outputfile.is_open())
	{
		outputfile << "ply" << endl;
		outputfile << "format ascii 1.0" << endl;
		outputfile << "element vertex " << mesh->numVertices() << endl;
		outputfile << "property double x" << endl;
		outputfile << "property double y" << endl;
		outputfile << "property double z" << endl;
		outputfile << "property double nx" << endl;
		outputfile << "property double ny" << endl;
		outputfile << "property double nz" << endl;
		if (mesh->numFaces() > 0)
		{
			hasFaces = true;
			outputfile << "element face " << mesh->numFaces() << endl;
			outputfile << "property list uchar int vertex_indices" << endl;
		}
		else
		{
			hasFaces = false;
		}
		outputfile << "end_header" << endl;
	}
	// calc face normal
	for (ConvMesh::MeshFaceIterator fIter(mesh); !fIter.end(); fIter++)
	{
		CFace * pF = *fIter;
		CVertex * pV1 = pF->halfedge()->vertex();
		CVertex * pV2 = pF->halfedge()->he_next()->vertex();
		CVertex * pV3 = pF->halfedge()->he_next()->he_next()->vertex();
		CPoint edgeVector1 = pV2->point() - pV1->point();
		CPoint edgeVector2 = pV3->point() - pV1->point();
		CPoint faceNormal = crossProduct(edgeVector1, edgeVector2);

		pF->normal() = faceNormal;
	}
	// calc vertex normal
	for (ConvMesh::MeshVertexIterator viter(mesh); !viter.end(); viter++)
	{
		CVertex * pV = *viter;
		// for debugging
		// cout << pV->id() << endl;
		CPoint vertexNormal;
		vertexNormal[0] = vertexNormal[1] = vertexNormal[2] = 0.0;
		for (ConvMesh::VertexFaceIterator vfIter(pV); !vfIter.end(); vfIter++)
		{
			CFace * pF = *vfIter;
			vertexNormal += pF->normal();
		}

		vertexNormal /= vertexNormal.norm();
		pV->normal() = vertexNormal;
	}
	for (ConvMesh::MeshVertexIterator viter(mesh); !viter.end(); viter++)
	{
		CVertex * pV = *viter;
		CPoint point = pV->point();
		CPoint verNormal = pV->normal();
		outputfile << point[0] << " " << point[1] << " " << point[2] << " " << verNormal[0] << " " << verNormal[1] << " " << verNormal[2] << endl;
	}

	// write faces to plyfile
	if (hasFaces)
	{
		for (ConvMesh::MeshFaceIterator fIter(mesh); !fIter.end(); fIter++)
		{
			CFace * face = *fIter;
			CHalfEdge * hf = face->halfedge();
			CVertex * v1 = hf->vertex();
			CVertex * v2 = hf->he_next()->vertex();
			CVertex * v3 = hf->he_next()->he_next()->vertex();
			outputfile << "3 " << v1->id()-1 << " " << v2->id()-1 << " " << v3->id()-1 << endl;
		}
	}

	outputfile.close();
	return 0;
}

