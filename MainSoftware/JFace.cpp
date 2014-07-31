#include "JFace.h"

JFace::JFace()
{
	vert1Id = vert2Id = vert3Id = 0;
	normalSet = false;
}

JFace::JFace(int v1, int v2, int v3)
{
	vert1Id = v1; vert2Id = v2; vert3Id = v3;
	normalSet = false;
}

JFace::~JFace()
{
}

void JFace::setFaceNormal(CPoint _normal)
{
	faceNormal = _normal;
	normalSet = true;
}

CPoint JFace::getFaceNormal()
{
	assert(normalSet);
	return faceNormal;
}
