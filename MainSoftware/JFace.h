#pragma once

#include "header\Point.h"

using namespace MeshLib;

class JFace
{
public:
	// constructor
	JFace();
	// overload constructor
	JFace(int, int, int);
	~JFace();
	// compute face normal
	void setFaceNormal(CPoint);
	// get face normal
	CPoint getFaceNormal();
private:
	bool normalSet;
protected:
	CPoint faceNormal;

public:
	int vert1Id, vert2Id, vert3Id;
};

