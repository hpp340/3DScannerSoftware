#include "stdafx.h"

#include "ConvMesh.h"

void MeshNormalize(ConvMesh * pMesh)
{

	CPoint center(0, 0, 0);
	for (ConvMesh::MeshVertexIterator viter(pMesh); !viter.end(); viter++)
	{
		CVertex * pV = *viter;
		center += pV->point();
	}

	center /= pMesh->numVertices();

	for (ConvMesh::MeshVertexIterator viter(pMesh); !viter.end(); viter++)
	{
		CVertex * pV = *viter;
		pV->point() -= center;
	}

	double scale = 0;

	for (ConvMesh::MeshVertexIterator viter(pMesh); !viter.end(); viter++)
	{
		CVertex * pV = *viter;
		CPoint p = pV->point();

		for (int i = 0; i < 3; i++)
		{
			scale = (scale > fabs(p[i])) ? scale : fabs(p[i]);
		}
	}

	for (ConvMesh::MeshVertexIterator viter(pMesh); !viter.end(); viter++)
	{
		CVertex * pV = *viter;
		pV->point() /= scale;
	}

	for (ConvMesh::MeshVertexIterator viter(pMesh); !viter.end(); viter++)
	{
		CVertex * pV = *viter;
		//pV->z() = std::complex<double>(pV->point()[0], pV->point()[1]);
	}
}

CPoint crossProduct(CPoint p1, CPoint p2)
{
	CPoint result;
	result[0] = p1[1] * p2[2] - p2[1] * p1[2];
	result[1] = p1[2] * p2[0] - p1[0] * p2[2];
	result[2] = p1[0] * p2[1] - p1[1] * p2[0];

	return result;
}