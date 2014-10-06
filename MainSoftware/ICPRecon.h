/* ICPRecon.h
This class is used to rigid align two pointcloud with ICP(Iterative Closest Point)
Closest point search is finished by KnnSearch using KDTree
Author: Jerome Jiang
Date: 10/1/2014
Updated: 10/6/2014
*/

/* Usage:
* Use the overload constructor to pass the point cloud to this class
* start the registration process by call the public method "startRegistration", return the updated rotation matrix and translation vector
* Other parameters include the iteration number and the option - whether using point to point minimization or point to plane minimization
*/

#pragma once

#include "header\PlyCloud.h"
#include "header\ANN\ANN.h"
//#include "header\ANNx.h"
#include "header\eigen\Eigen\Dense"
#include "header\eigen\Eigen\SVD"

enum class ICPOption { POINT_TO_POINT, POINT_TO_PLANE };

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
	// icpOpt: fit point to point or point to plane
	void startRegistration(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, int _iterationTimes, ICPOption icpOpt);

protected:
	// if the class has received the data cloud and target cloud
	bool hasPtCloud;
	PlyCloud * dataCloud;
	PlyCloud * targetCloud;
	// kdtree used for searching the closest point
	ANNkd_tree * kdTree;
	// number of nearest points
	int numNearNeigh;
	// number of neighbor used for normal estimation
	int numNeighbor;
	Eigen::MatrixX3d normalMat;

private:
	// compute the transformation for current iteration in the setting of point to plane
	// rotMat(changed): return the rotation matrix
	// transVec(changed): return the translation vector
	// targetPts & dataPts: corresponded i.e. targetPts[i] is the closest point to dataPts[i]
	void computePtToPointTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts);

	// compute the transformation for current iteration in the setting of point to plane
	// additional parameter: matched normal matrix (for target point cloud)
	void computePtToPlaneTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts, Eigen::MatrixX3d normalMat);

	// compute the Root Mean Squre(RMS) error between p1 and p2
	double RMSError(Eigen::MatrixX3d p1, Eigen::MatrixX3d p2);

	// estimate the normal of target point cloud so that we can fit point to plane
	void normalEstimate();
};

