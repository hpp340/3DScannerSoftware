#pragma once

#include "header\PlyCloud.h"
#include "header\ANN\ANN.h"
//#include "header\ANNx.h"
#include "header\eigen\Eigen\Dense"
#include "header\eigen\Eigen\SVD"

enum class ICPOption { POINT_TO_POINT, POINT_TO_PLANE };

/*! ICPRecon.h
* \brief This class is used to rigid align two pointcloud with ICP(Iterative Closest Point)
* Closest point search is finished by KNN-Search using KDTree, depending on library ANN
* \author: Jerome Jiang
* \date: 10/1/2014
* Usage:
* Use the overload constructor to pass the point cloud to this class
* start the registration process by call the public method "startRegistration", return the updated rotation matrix and translation vector
* Other parameters include the iteration number and the option - whether using point to point minimization or point to plane minimization
*/
class ICPRecon
{
public:

	/// Constructor
	ICPRecon();
	/// Overload constructor.
	/// When dim is not specified, equals 3 by default.
	/// \param _dataCloud the point cloud that to be aligned
	/// \param _targetCloud the point cloud that serves as the target, i.e. it is fixed.
	ICPRecon(PlyCloud * _dataCloud, PlyCloud * _targetCloud);

	/// Deconstructor
	~ICPRecon();

	/// Start the ICP registration process
	/// \param rotMat return the rotation matrix which is 3*3
	/// \param transVec return the translation vector which is 3*1
	/// \param _iterationTimes the number of iterations
	/// \param icpOpt fit point to point or point to plane
	void startRegistration(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, int _iterationTimes, ICPOption icpOpt);

protected:
	/// flag indicating if the class has received the data cloud and target cloud
	bool hasPtCloud;

	/// Data point cloud, which is gonna to be transformed
	PlyCloud * dataCloud;

	/// Target point cloud, fixed.
	PlyCloud * targetCloud;


	/// kdtree used for searching the closest point
	ANNkd_tree * kdTree;

	/// Number of nearest points. should be 1 in the setting of ICP
	int numNearNeigh;

	/// Number of neighbor used for normal estimation. Only used in POINT_TO_PLANE distance metric.
	int numNeighbor;

	/// Stores the normal for vertices. a N*3 matrix where N is the number of all vertices.
	Eigen::MatrixX3d normalMat;

private:

	/// Compute the transformation for current iteration in the setting of point to point.
	/// \param rotMat (changed)return the rotation matrix
	/// \param transVec (changed)return the translation vector
	/// \param targetPts,dataPts: corresponded i.e. targetPts[i] is the closest point to dataPts[i]
	void computePtToPointTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts);

	/// Compute the transformation for current iteration in the setting of point to plane
	/// \param normalMat matched normal matrix (for target point cloud)
	void computePtToPlaneTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts, Eigen::MatrixX3d normalMat);

	/// Compute the Root Mean Squre(RMS) error between p1 and p2
	double RMSError(Eigen::MatrixX3d p1, Eigen::MatrixX3d p2);

	/// Estimate the normal of target point cloud so that we can fit point to plane
	void normalEstimate();
};

