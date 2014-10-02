/* ICPRecon.h
This class is used to rigid align two pointcloud with ICP(Iterative Closest Point)
Closest point search is finished by KnnSearch using KDTree
Author: Jerome Jiang
Date: 10/1/2014
*/

#pragma once

#include "header\PlyCloud.h"
#include "header\ANN\ANN.h"
#include "header\eigen\Eigen\Dense"

class ICPRecon
{
public:
	ICPRecon();
	// overload constructor
	// receive two point clouds
	ICPRecon(PlyCloud *, PlyCloud *);
	~ICPRecon();

	// start the ICP registration process
	void startRegistration(Eigen::Matrix<double, 4, 4> * rotMat, int _iterationTimes);

private:
	bool hasPtCloud;
	PlyCloud * dataCloud;
	PlyCloud * targetCloud;
};

