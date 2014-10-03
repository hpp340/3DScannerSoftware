#include "ICPRecon.h"


ICPRecon::ICPRecon()
{
	hasPtCloud = false;
	numNearNeigh = 1;
}

// overload constructor
ICPRecon::ICPRecon(PlyCloud * _dataCloud, PlyCloud * _targetCloud) :
dataCloud(_dataCloud), targetCloud(_targetCloud)
{
	hasPtCloud = true;
	numNearNeigh = 1;
}

void ICPRecon::startRegistration(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, int _numIteration)
{
	if (! hasPtCloud)
	{
		std::cout << "Error: No Point Cloud to align." << std::endl;
		return;
	}
	else
	{
		ANNpointArray targetPts;
		int targetNum = targetCloud->get_vertex_num();
		// allocate space
		targetPts = annAllocPts(targetNum, 3);
		//ANNpointArray dataPts;
		int dataNum = dataCloud->get_vertex_num();
		//dataPts = annAllocPts(dataNum, 3);

		// using eigen matrix to represent the data and target points for conveniently transform mesh
		Eigen::MatrixX3d targetMat;
		Eigen::MatrixX3d dataMat;
		Eigen::MatrixX3d transedDataMat;

		std::vector<JVertex *> dataVertexList = dataCloud->getJVertexList();
		std::vector<JVertex *> targetVertexList = targetCloud->getJVertexList();

		// fill in the ANN point array
		for (size_t i = 0; i < targetVertexList.size(); i++)
		{
			CPoint currPt = targetVertexList[i]->getPoint();
			for (int d = 0; d < 3; d++)
			{
				targetPts[i][d] = currPt[d];
				targetMat(i, d) = currPt[d];
			}
		}

		for (size_t i = 0; i < dataVertexList.size(); i++)
		{
			CPoint currPt = dataVertexList[i]->getPoint();
			for (int d = 0; d < 3; d++)
			{
				//dataPts[i][d] = currPt[d];
				dataMat(i, d) = currPt[d];
			}
		}

		// initialize the transedDataMat
		transedDataMat = dataMat;

		ANNidxArray nnIdx = new ANNidx[1];
		ANNdistArray nnDists = new ANNdist[1];

		ANNidxArray matchResult = new ANNidx[dataNum];
		ANNdistArray matchDists = new ANNdist[dataNum];

		// construct the kdtree
		kdTree = new ANNkd_tree(targetPts, targetNum, 3);

		// enter the main iterations
		for (int k = 0; k < _numIteration; k++)
		{
			std::cout << "ICP Iteration " << k << std::endl;
			for (int i = 0; i < dataNum; i++)
			{
				ANNpoint currDataPt = annAllocPt(3);
				for (int j = 0; j < 3; j++)
				{
					currDataPt[j] = transedDataMat(i, j);
				}
				kdTree->annkSearch(currDataPt, 1, nnIdx, nnDists);
				matchResult[i] = nnIdx[0];
				matchDists[i] = nnDists[0];
			}

			Eigen::MatrixX3d matchedTargetPtMat;
			for (int i = 0; i < dataNum; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					matchedTargetPtMat(i, j) = targetMat(matchResult[i], j);
				}
			}
			Eigen::Matrix3d currRotMat;
			Eigen::Vector3d currTransVec;
			computeTransformation(currRotMat, currTransVec, matchedTargetPtMat, transedDataMat);
			rotMat = rotMat * currRotMat;
			transVec = currRotMat * transVec + currTransVec;
			// apply the latest transformation
			transedDataMat = (rotMat * transedDataMat.transpose() + currTransVec).transpose();

			double rmsErr = RMSError(matchedTargetPtMat, transedDataMat);
			std::cout << "RMS Error for Iteration " << k << " : " << rmsErr << std::endl;
		}
	}
}

ICPRecon::~ICPRecon()
{
}

void ICPRecon::computeTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts)
{
	// find the points center
	Eigen::Vector3d dataMean;
	Eigen::Vector3d targetMean;
	for (int j = 0; j < dataPts.cols(); j++)
	{
		double sum = 0.0;
		for (int i = 0; i < dataPts.rows(); j++)
		{
			sum += dataPts(i, j);
		}
		double mean = sum / dataPts.rows();
		dataMean[j] = mean;
	}

	for (int j = 0; j < targetPts.cols(); j++)
	{
		double sum = 0.0;
		for (int i = 0; i < targetPts.rows(); j++)
		{
			sum += targetPts(i, j);
		}
		double mean = sum / targetPts.rows();
		targetMean[j] = mean;
	}

	Eigen::MatrixX3d dataMark;
	Eigen::MatrixX3d targetMark;

	for (int i = 0; i < dataPts.rows(); i++)
	{
		for (int j = 0; j < dataPts.cols(); j++)
		{
			dataMark(i, j) = dataPts(i, j) - dataMean(j);
		}
	}

	for (int i = 0; i < targetPts.rows(); i++)
	{
		for (int j = 0; j < targetPts.cols(); j++)
		{
			targetMark(i, j) = targetPts(i, j) - targetMean(j);
		}
	}

	Eigen::Matrix3d N;
	N = dataMark.transpose() * targetMark;
	Eigen::JacobiSVD<Eigen::Matrix3d> svdDecomposition = N.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV);
	Eigen::Matrix3d U = svdDecomposition.matrixU();
	Eigen::Matrix3d V = svdDecomposition.matrixV();

	Eigen::DiagonalMatrix<double, Eigen::Dynamic> diagM(3); // (U * V.transpose()).determinant());
	diagM.diagonal()(0) = 1.0; 
	diagM.diagonal()(1) = 1.0; 
	diagM.diagonal()(2) = (U * V.transpose()).determinant();

	rotMat = V * diagM * U.transpose();
	transVec = targetMean - rotMat * dataMean;
}

double ICPRecon::RMSError(Eigen::MatrixX3d p1, Eigen::MatrixX3d p2)
{
	Eigen::MatrixX3d d = p1 - p2;
	Eigen::VectorXd dsqVec;
	for (int i = 0; i < d.rows(); i++)
	{
		double sum = 0;
		for (int j = 0; j < d.cols(); j++)
		{
			sum += d(i, j) * d(i, j);
		}
		dsqVec(i) = sum;
	}

	double rmsErr = sqrt(dsqVec.mean());

	return rmsErr;
}
