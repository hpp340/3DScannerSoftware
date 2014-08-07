#include "JFace.h"

JFace::JFace()
{
	vert1Id = vert2Id = vert3Id = 0;
	normalSet = false;
	textureSet = false;
}

JFace::JFace(int id)
{
	faceId = id;
	vert1Id = vert2Id = vert3Id = 0;
	normalSet = false;
	textureSet = false;
}

JFace::JFace(int v1, int v2, int v3)
{
	vert1Id = v1; vert2Id = v2; vert3Id = v3;
	normalSet = false;
	textureSet = false;
}

JFace::JFace(int v1, int v2, int v3, int t1, int t2, int t3)
{
	vert1Id = v1; vert2Id = v2; vert3Id = v3;
	texture1Id = t1; texture2Id = t2, texture3Id = t3;
	normalSet = false;
	textureSet = true;
}

JFace::~JFace()
{
}

void JFace::setFaceNormal(CPoint _normal)
{
	faceNormal = _normal;
	normalSet = true;
}

void JFace::setFaceId(int id)
{
	faceId = id;
}

CPoint JFace::getFaceNormal()
{
	assert(normalSet);
	return faceNormal;
}