#include "PoissonRecon.h"


PoissonRecon::PoissonRecon()
{
	std::cout << "PoissonRecon:PoissonRecon Constructor" << std::endl;
	pointCloud = new PlyCloud();
	reconSurface = new PlyCloud();
	depth = 8;
	minDepth = 5;
	kernelDepth = depth - 2;
	samplesPerNode = 1;
	adaptiveExpo = 1;
	isoDivide = 8;
	pointWeight = 4.0f;
	scale = 1.1f;
	boundaryType = 1;
	solverDivide = 8;
	minIter = 24;
	solveAccuracy = 1e-3;
	maxSolveDepth = depth;
	fixedIters = -1;
}


PoissonRecon::~PoissonRecon()
{
}

void PoissonRecon::acceptPointCloud(PlyCloud * outerPly)
{
	free(pointCloud);
	if (outerPly)
	{
		pointCloud = outerPly;
	}

}


void PoissonRecon::acceptPointCloudFromFile(std::string _filename)
{
	std::cout << "PoissonRecon:acceptPointCloudFromFile" << std::endl;
	filename = _filename;
}


PlyCloud * PoissonRecon::getSurface()
{
	return reconSurface;
}

	