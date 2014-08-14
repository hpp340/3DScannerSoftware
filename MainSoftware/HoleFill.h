/* This class is used to perform the Hole Filling Algorithm
* Using M. Botsch, M. Pauly, L. Kobbelt, P. Alliez, B. Levy,
* "Geometric Modeling Based on Polygonal Meshes"
* held at SIGGRAPH 2007, San Diego, and Eurographics 2008, Crete.
* Author: Shawn
* Date: 8/11/2014
*/

#ifndef Hole_Fill_H
#define Hole_Fill_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#endif // _WIN32
#include<iostream>
#include"header\holefill\Core\IO\MeshIO.hh"
#include"header\holefill\Core\Mesh\Types\TriMesh_ArrayKernelT.hh"

#include "HoleFiller.hh"

//=============================================================================

typedef OpenMesh::TriMesh_ArrayKernelT<OpenMesh::DefaultTraits> MyMesh;

//=============================================================================
class HoleFill
{
public:
	HoleFill(){}
	~HoleFill(){}
	//read in mesh
	void acceptReadinFilename(std::string);

private:
	//name of the mesh read in
	std::string filename;


public:
	void startFill()
	{
		MyMesh mesh;
		// read a mesh
		if (!OpenMesh::IO::read_mesh(mesh, filename))
		{
			std::cerr << "Error: Could not read mesh\n";
		}

		// fill the holes

		HoleFiller< MyMesh > hf(mesh);
		hf.fill_all_holes(3);

		// write result
		std::cerr << "Saving result ... ";


		if (!OpenMesh::IO::write_mesh(mesh, "holefilled.obj"))
		{
			std::cerr << "Error: Could not write mesh\n";
		}

		std::cerr << "ok\n";
		//getchar();
	}

};

#endif