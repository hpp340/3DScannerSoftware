#include "JFace.h"

JFace::JFace()
{
	point1[0] = point1[1] = point1[2] = 0.0;
	point2[0] = point2[1] = point2[2] = 0.0;
	point3[0] = point3[1] = point3[2] = 0.0;
	vert1Id = vert2Id = vert3Id = 0;
}

JFace::JFace(CPoint p1, CPoint p2, CPoint p3)
{
	point1[0] = p1[0]; point1[1] = p1[1]; point1[2] = p1[2];
	point2[0] = p2[0]; point2[1] = p2[1]; point2[2] = p2[2];
	point3[0] = p3[0]; point3[1] = p2[1]; point3[2] = p3[2];
	vert1Id = vert2Id = vert3Id = 0;
}

JFace::JFace(int v1, int v2, int v3)
{
	vert1Id = v1; vert2Id = v2; vert3Id = v3;
	point1[0] = point1[1] = point1[2] = 0.0;
	point2[0] = point2[1] = point2[2] = 0.0;
	point3[0] = point3[1] = point3[2] = 0.0;
}

JFace::~JFace()
{
}
