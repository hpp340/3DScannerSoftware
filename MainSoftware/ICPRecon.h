/* ICPRecon.h
This class is used to rigid align two pointcloud with ICP(Iterative Closest Point)
Closest point search is finished by KnnSearch using KDTree
Author: Jerome Jiang
Date: 10/1/2014
*/

#pragma once

#include "header\PlyCloud.h"
#include "header\ANN\ANN.h"
//#include "header\ANNx.h"
#include "header\eigen\Eigen\Dense"
#include "header\eigen\Eigen\SVD"

class ICPRecon
{
public:
	ICPRecon();
	// overload constructor
	// when dim is not specified, equals 3 by default
	ICPRecon(PlyCloud *, PlyCloud *);
	~ICPRecon();

	// start the ICP registration process
	// rotMat: return the rotation matrix which is 3*3
	// transVec: return the translation vector which is 3*1
	// _iterationTimes: the number of iterations
	void startRegistration(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, int _iterationTimes);

private:
	// if the class has received the data cloud and target cloud
	bool hasPtCloud;
	PlyCloud * dataCloud;
	PlyCloud * targetCloud;
	// kdtree used for searching the closest point
	ANNkd_tree * kdTree;
	// number of nearest points
	int numNearNeigh;

private:
	// computet the transformation for current iteration
	// rotMat(changed): return the rotation matrix
	// transVec(changed): return the translation vector
	// targetPts & dataPts: corresponded i.e. targetPts[i] is the closest point to dataPts[i]
	void computeTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts);

	// compute the Root Mean Squre(RMS) error between p1 and p2
	double RMSError(Eigen::MatrixX3d p1, Eigen::MatrixX3d p2);
};

