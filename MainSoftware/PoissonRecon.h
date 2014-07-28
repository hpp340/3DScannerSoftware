/* This class is used to perform the Poisson Surface Reconstruction
 * Using Houge's paper Poisson Surface Reconstruction and the code along with it
 * Author: Jerome
 * Date: 7/27/2014
*/

#ifndef POISSON_RECON_H
#define POISSON_RECON_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#ifdef _WIN32
#include <Windows.h>
#include <Psapi.h>
#endif // _WIN32
#include "header\recon\Time.h"
#include "header\recon\MarchingCubes.h"
#include "header\recon\Octree.h"
#include "header\recon\SparseMatrix.h"
#include "header\recon\CmdLineParser.h"
#include "header\recon\PPolynomial.h"
#include "header\recon\Ply.h"
#include "header\recon\MemoryUsage.h"
#include "omp.h"
#include <stdarg.h>
#include <string>

#include "header\PlyCloud.h"

#include "header\recon\MultiGridOctreeData.h"

#define ITERATION_POWER 1.0/3
#define MEMORY_ALLOCATOR_BLOCK_SIZE 1<<12
//#define MEMORY_ALLOCATOR_BLOCK_SIZE 0
#define SPLAT_ORDER 2
#define MEMORY_USAGE_FIX 1

class PoissonRecon
{
public:
	PoissonRecon();
	~PoissonRecon();

	// add point cloud
	void acceptPointCloud(PlyCloud *);
	// add point cloud from file
	void acceptPointCloudFromFile(std::string);
	// return point cloud
	PlyCloud * getSurface();


private:
	std::string filename;
	PlyCloud * pointCloud;
	PlyCloud * reconSurface;
	int depth, minDepth, kernelDepth, samplesPerNode, adaptiveExpo;
	int isoDivide;
	int boundaryType;
	int solverDivide;
	int minIter;
	int fixedIters;
	double solveAccuracy;
	int maxSolveDepth;
	Real pointWeight, scale;
	

public:
	// start recon
	template<int Degree, class Vertex, bool OutputDensity>
	void startRecon()
	{
		std::cout << "PoissonRecon: startRecon" << std::endl;
		char * outputFilename = "reconOutput.ply";
		// estblish an Octree
		XForm4x4< Real > xForm, iXForm;
		xForm = XForm4x4< Real >::Identity();
		iXForm = xForm.inverse();

		Real isoValue = 0;

		Octree< Degree, OutputDensity > tree;
		tree.threads = omp_get_num_procs();
		std::cout << "PoissonRecon:setBSplineData" << std::endl;
		tree.setBSplineData(depth, boundaryType);

		OctNode< TreeNodeData< OutputDensity >, Real >::SetAllocator(MEMORY_ALLOCATOR_BLOCK_SIZE);

		//double maxMemoryUsage;
		tree.maxMemoryUsage = 0;
		printf("%s\n", filename);
		const char * _filenameConst = filename.c_str();
		char * _filename = new char[strlen(_filenameConst) + 1];
		strcpy(_filename, _filenameConst);
		std::cout << "filename" << std::endl;
		printf("%s\n", _filename);
		std::cout << "PoissonRecon:setTree" << std::endl;
		tree.setTree(_filename, depth, minDepth, kernelDepth, Real(samplesPerNode), scale, false, false, pointWeight, adaptiveExpo, xForm);
		std::cout << "PoissonRecon:ClipTre" << std::endl;
		tree.ClipTree();
		std::cout << "PoissonRecon:finalize" << std::endl;
		tree.finalize(isoDivide);

		std::cout << "PoissonRecon:SetLaplacianConstraints" << std::endl;
		tree.SetLaplacianConstraints();

		std::cout << "PoissonRecon:LaplacianMatrixIteration" << std::endl;
		tree.LaplacianMatrixIteration(solverDivide, false, minIter, solveAccuracy, maxSolveDepth, fixedIters);

		CoredFileMeshData< Vertex > mesh;
		std::cout << "PoissonRecon:GetIsoValue" << std::endl;	
		isoValue = tree.GetIsoValue();
		std::cout << "PoissonRecon:GetTriangles" << std::endl;
		tree.GetMCIsoTriangles(isoValue, isoDivide, &mesh, 0, 1, true, false);
		std::cout << "PoissonRecon:PlyWritePolygons" << std::endl;
		PlyWritePolygons(outputFilename, &mesh, PLY_ASCII, NULL, 0, iXForm);
	}
};

#endif

