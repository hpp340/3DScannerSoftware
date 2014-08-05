#pragma once

#include "header\Point.h"

using namespace MeshLib;

class JFace
{
public:
	// constructor
	JFace();
	// overload constructor, three vertex id
	JFace(int, int, int);
	// overload constructor, three vertex id, three texture id
	JFace(int, int, int, int, int, int);

	~JFace();
	// compute face normal
	void setFaceNormal(CPoint);
	// get face normal
	CPoint getFaceNormal();
	// if normal set
	bool isNormalSet() { return normalSet; };
	// if texture id set
	bool isTextureSet() { return normalSet; };

private:
	bool normalSet;
	bool textureSet;
protected:
	CPoint faceNormal;

public:
	int vert1Id, vert2Id, vert3Id;
	int texture1Id, texture2Id, texture3Id;
};

