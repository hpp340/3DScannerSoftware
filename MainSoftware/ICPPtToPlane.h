/* ICPPtToPlane.h 
This class is used for ICP registration using point-plane optimization
Author: Jerome Jiang
Date: 10/6/2014
*/

#pragma once

#include "ICPRecon.h"

class ICPPtToPlane : public ICPRecon
{
public:
	ICPPtToPlane();
	// overload constructor
	ICPPtToPlane(PlyCloud * _data, PlyCloud * _target);
	~ICPPtToPlane();

protected:
	// overwrite
	void computeTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts);

private:
	// estimate the normal for target point cloud
	void normEstimate();

private:
	// used to store the normals
	Eigen::MatrixX3d normMat;
	// used for estimate the normal
	int numNeighbor;
};

