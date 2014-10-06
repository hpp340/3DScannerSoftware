#include "ICPPtToPlane.h"


ICPPtToPlane::ICPPtToPlane()
{
	hasPtCloud = false;
	numNearNeigh = 1;
	numNeighbor = 4;
}

// overload constructor
ICPPtToPlane::ICPPtToPlane(PlyCloud * _dataCloud, PlyCloud * _targetCloud)
{
	dataCloud = _dataCloud;
	targetCloud = _targetCloud;
	hasPtCloud = true;
	numNearNeigh = 1;
	numNeighbor = 4;

	int targetNum = targetCloud->get_vertex_num();
	normMat.resize(targetNum, 3);
	if (! targetCloud->hasNormal())
	{
		normEstimate();
	}
	else
	{
		// fill in norm mat
		std::vector<JVertex *> targetVertList = targetCloud->getJVertexList();
		for (int i = 0; i < normMat.rows(); i++)
		{
			JVertex * currVert = targetVertList[i];
			CPoint currPt = currVert->getPoint();
			for (int j = 0; j < normMat.cols(); j++)
			{
				normMat(i, j) = currPt(j);
			}
		}
	}
}

ICPPtToPlane::~ICPPtToPlane()
{
}

void ICPPtToPlane::computeTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts)
{
	Eigen::Vector3d dataMean;
	Eigen::Vector3d targetMean;
	dataMean = dataPts.colwise().mean();
	targetMean = targetPts.colwise().mean();

	for (int i = 0; i < dataPts.rows(); i++)
	{
		Eigen::Vector3d currPt;
		Eigen::Vector3d currNorm;
		for (int j = 0; j < dataPts.cols(); j++)
		{
			currPt(j) = dataPts(i, j);
		}
		Eigen::Vector3d crossProduct;
		
	}

}

void ICPPtToPlane::normEstimate()
{
	// construct kdtree with target points
	ANNpointArray annTargetPts;
	int targetNum = targetCloud->get_vertex_num();
	annTargetPts = annAllocPts(targetNum, 3);
	// fill in ann point array
	std::vector<JVertex *> targetVertList = targetCloud->getJVertexList();
	for (size_t i = 0; i < targetVertList.size(); i++)
	{
		CPoint currPt = targetVertList[i]->getPoint();
		for (int d = 0; d < 3; d++)
		{
			annTargetPts[i][d] = currPt[d];
		}
	}

	ANNkd_tree * normKDTree = new ANNkd_tree(annTargetPts, targetNum, 3);

	for (int i = 0; i < targetNum; i++)
	{
		ANNpoint currSearchedPt = annAllocPt(3);
		ANNidxArray nnIdxs = new ANNidx[numNeighbor + 1];
		ANNdistArray nnDists = new ANNdist[numNeighbor + 1];
		for (int d = 0; d < 3; d++)
		{
			currSearchedPt[d] = annTargetPts[i][d];
		}
		normKDTree->annkSearch(currSearchedPt, numNeighbor + 1, nnIdxs, nnDists);
		// fill in the neighbor points to eigen matrix for computing
		Eigen::MatrixX3d neighPts;
		neighPts.resize(numNeighbor, 3);
		for (int ii = 0; ii < numNeighbor; ii++)
		{
			for (int d = 0; d < 3; d++)
			{
				neighPts(ii + 1, d) = annTargetPts[nnIdxs[ii + 1]][d];
			}
		}
		Eigen::RowVector3d sumNeighPts = neighPts.colwise().sum();
		Eigen::RowVector3d neighPtsMean = sumNeighPts / numNeighbor;
		// substract the mean value
		for (int ii = 0; ii < neighPts.rows(); ii++)
		{
			for (int j = 0; j < neighPts.cols(); j++)
			{
				neighPts(ii, j) = neighPts(ii, j) - neighPtsMean(j);
			}
		}

		Eigen::Matrix3d P;
		P = neighPts.transpose() * neighPts;
		Eigen::EigenSolver<Eigen::Matrix3d> eigenSol(P);
		Eigen::Vector3d eigenValue = eigenSol.eigenvalues();
		Eigen::Matrix3d eigenVector = eigenSol.eigenvectors();
		Eigen::Vector3d::Index minEigenValueIdx;
		eigenValue.minCoeff(&minEigenValueIdx);
		for (int d = 0; d < 3; d++)
		{
			normMat(i, d) = eigenVector(d, minEigenValueIdx);
		}
	}
}