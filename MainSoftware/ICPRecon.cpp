#include "ICPRecon.h"


ICPRecon::ICPRecon()
{
	hasPtCloud = false;
	numNearNeigh = 1;
	numNeighbor = 4;
}

// overload constructor
ICPRecon::ICPRecon(PlyCloud * _dataCloud, PlyCloud * _targetCloud)
{
	dataCloud = _dataCloud;
	targetCloud = _targetCloud;
	hasPtCloud = true;
	numNearNeigh = 1;
}

void ICPRecon::startRegistration(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, int _numIteration, ICPOption icpOpt)
{
	if (! hasPtCloud)
	{
		std::cout << "Error: No Point Cloud to align." << std::endl;
		return;
	}
	else
	{
		if (icpOpt == ICPOption::POINT_TO_PLANE)
		{
			normalMat.resize(targetCloud->get_vertex_num(), 3);
			if (! targetCloud->hasNormal())
			{
				normalEstimate();
			}
			else
			{
				std::vector<JVertex *> targetVertList = targetCloud->getJVertexList();
				for (int i = 0; i < normalMat.rows(); i++)
				{
					CPoint currPt = targetVertList[i]->getPoint();
					for (int j = 0; j < normalMat.cols(); j++)
					{
						normalMat(i, j) = currPt(j);
					}
				}
			}
		}

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

		targetMat.resize(targetVertexList.size(), 3);
		dataMat.resize(dataVertexList.size(), 3);

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
			matchedTargetPtMat.resize(dataMat.rows(), dataMat.cols());

			for (int i = 0; i < dataNum; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					matchedTargetPtMat(i, j) = targetMat(matchResult[i], j);
				}
			}

			Eigen::MatrixX3d matchedNormalMat;

			// minimize the energy to compute the transformation
			Eigen::Matrix3d currRotMat;
			Eigen::Vector3d currTransVec;
			switch (icpOpt)
			{
			case ICPOption::POINT_TO_POINT:
				computePtToPointTransformation(currRotMat, currTransVec, matchedTargetPtMat, transedDataMat);
				break;
			case ICPOption::POINT_TO_PLANE:
				matchedNormalMat.resize(dataMat.rows(), dataMat.cols());
				for (int i = 0; i < dataNum; i++)
				{
					for (int j = 0; j < 3; j++)
					{
						matchedNormalMat(i, j) = normalMat(matchResult[i], j);
					}
				}
				computePtToPlaneTransformation(currRotMat, currTransVec, matchedTargetPtMat, transedDataMat, matchedNormalMat);
				break;
			default:
				break;
			}
			rotMat = currRotMat * rotMat;
			transVec = currRotMat * transVec + currTransVec;

			// apply the latest transformation
			// the following code block is to realize the following transformation
			// transedDataMat = (rotMat * transedDataMat.transpose() + currTransVec).transpose();
			Eigen::Matrix3Xd tempMat;
			tempMat.resize(3, transedDataMat.rows());
			tempMat = rotMat * (dataMat.transpose());
			for (int i = 0; i < transedDataMat.rows(); i++)
			{
				for (int j = 0; j < transedDataMat.cols(); j++)
				{
					transedDataMat(i, j) = tempMat(j, i) + transVec(j);
				}
			}

			double rmsErr = RMSError(matchedTargetPtMat, transedDataMat);
			std::cout << "RMS Error for Iteration " << k << " : " << rmsErr << std::endl;
		}
	}
}

ICPRecon::~ICPRecon()
{
}

void ICPRecon::computePtToPointTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts)
{
	// find the points center
	Eigen::Vector3d dataMean;
	Eigen::Vector3d targetMean;
	for (int j = 0; j < dataPts.cols(); j++)
	{
		double sum = 0.0;
		for (int i = 0; i < dataPts.rows(); i++)
		{
			sum += dataPts(i, j);
		}
		double mean = sum / dataPts.rows();
		dataMean[j] = mean;
	}

	for (int j = 0; j < targetPts.cols(); j++)
	{
		double sum = 0.0;
		for (int i = 0; i < targetPts.rows(); i++)
		{
			sum += targetPts(i, j);
		}
		double mean = sum / targetPts.rows();
		targetMean[j] = mean;
	}

	Eigen::MatrixX3d dataMark;
	Eigen::MatrixX3d targetMark;
	dataMark.resize(dataPts.rows(), dataPts.cols());
	targetMark.resize(targetPts.rows(), targetPts.cols());

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
			targetMark(i, j) = (targetPts(i, j) - targetMean(j)) / targetPts.rows();
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

void ICPRecon::computePtToPlaneTransformation(Eigen::Matrix<double, 3, 3> & rotMat, Eigen::Vector3d & transVec, Eigen::MatrixX3d targetPts, Eigen::MatrixX3d dataPts, Eigen::MatrixX3d normalMat)
{
	// cross product of correspondent point and normal
	// normal is of the target points
	// i.e. C = dataPts x normalMat
	int dataNum = (int)dataPts.rows();
	Eigen::MatrixX3d C;
	C.resize(dataNum, 3);
	for (int i = 0; i < dataNum; i++)
	{
		Eigen::Vector3d currData, currNorm;
		currData << dataPts(i, 0), dataPts(i, 1), dataPts(i, 2);
		currNorm << normalMat(i, 0), normalMat(i, 1), normalMat(i, 2);
		Eigen::Vector3d currResult = currData.cross(currNorm);
		for (int d = 0; d < 3; d++)
		{
			C(i, d) = currResult(d);
		}
	}
	Eigen::MatrixX3d CN;
	CN.resize(dataNum, 6);
	for (int i = 0; i < dataNum; i++)
	{
		for (int d = 0; d < 3; d++)
		{
			CN(i, d) = C(i, d);
			CN(i, d + 3) = normalMat(i, d);
		}
	}

	Eigen::Matrix3d CC = CN.transpose() * CN;

	Eigen::MatrixX3d tempSub;
	tempSub.resize(dataNum, 3);
	tempSub = dataPts - targetPts;
	Eigen::VectorXd b;
	b.resize(6);
	for (int i = 0; i < 6; i++)
	{
		double sumB = 0.0;
		for (int r = 0; r < tempSub.rows(); r++)
		{
			for (int c = 0; c < tempSub.cols(); c++)
			{
				double tempMul = tempSub(r, c) * CN(r, i) * normalMat(r, c);
				sumB += tempMul;
			}
		}
		// dont forget to take the negative
		b(i) = -sumB;
	}

	Eigen::Vector3d X;
	X = CC.fullPivLu().solve(b);

	double cx = cos(X(0));
	double cy = cos(X(1));
	double cz = cos(X(2));
	double sx = sin(X(0));
	double sy = sin(X(1));
	double sz = sin(X(2));

	rotMat(0, 0) = cy*cz; rotMat(0, 1) = cz*sx*sy - cx*sz; rotMat(0, 2) = cx*cz*sy + sx*sz;
	rotMat(1, 0) = cy*sz; rotMat(1, 1) = cx*cz + sx*sy*sz; rotMat(1, 2) = cx*sy*sz - cz*sx;
	rotMat(2, 0) = -sy;   rotMat(2, 1) = cy*sx;            rotMat(2, 2) = cx*cy;

	transVec(0) = X(3);
	transVec(1) = X(4);
	transVec(2) = X(5);

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

void ICPRecon::normalEstimate()
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
		Eigen::Vector3cd eigenValueComplex = eigenSol.eigenvalues();
		Eigen::Vector3d eigenValue;
		eigenValue << eigenValueComplex(0).real(), eigenValueComplex(1).real(), eigenValueComplex(2).real();
		Eigen::Matrix3cd eigenVectorComplex = eigenSol.eigenvectors();
		Eigen::Matrix3d eigenVector;
		for (int ii = 0; ii < 3; ii++)
		{
			for (int j = 0; j < 3; j++)
			{
				eigenValue(ii, j) = eigenVectorComplex(ii, j).real();
			}
		}

		Eigen::Vector3d::Index minEigenValueIdx;
		eigenValue.minCoeff(&minEigenValueIdx);
		for (int d = 0; d < 3; d++)
		{
			normalMat(i, d) = eigenVector(d, minEigenValueIdx);
		}
	}
}