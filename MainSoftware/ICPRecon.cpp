#include "ICPRecon.h"


ICPRecon::ICPRecon()
{
	hasPtCloud = false;
}

// overload constructor
ICPRecon::ICPRecon(PlyCloud * _dataCloud, PlyCloud * _targetCloud) : 
dataCloud(_dataCloud), targetCloud(_targetCloud)
{
	hasPtCloud = true;
}

void ICPRecon::startRegistration(Eigen::Matrix<double, 4, 4> * rotMat, int _numIteration)
{
	if (! hasPtCloud)
	{
		return;
	}
	else
	{
		// enter the main iterations
		for (int k = 0; k < _numIteration; k++)
		{
			std::cout << "ICP Iteration " << k << std::endl;
			
		}
	}
}

ICPRecon::~ICPRecon()
{
}


