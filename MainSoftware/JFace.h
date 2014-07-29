#pragma once

#include "header\Point.h"

using namespace MeshLib;

class JFace
{
public:
	// constructor
	JFace();
	// overload constructor
	JFace(CPoint, CPoint, CPoint);
	// overload constructor
	JFace(int, int, int);
	~JFace();

	CPoint point1, point2, point3;
	int vert1Id, vert2Id, vert3Id;
};

