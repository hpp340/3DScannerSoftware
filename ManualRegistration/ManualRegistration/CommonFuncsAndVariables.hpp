#pragma once

#include <stdlib.h>
#include "header\glut.h"
#include "header\glm\glm.hpp"
#include "header\glm\gtx\norm.hpp"
#include "header\PlyCloud.h"
#include "header\Point.h"

#include "stdafx.h"
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <vector>

#define WIDTH 1280	// width of glut window
#define HEIGHT 800	// height of glut window
#define PI 3.14159265	// pi
#define FOVY 80		// view angle used for gluLookAt
#define ZNEAR 1.0	// z near plane of view field
#define ZFAR 5.0	// z far plane of view field

const char *filename1, *filename2;
int mouseX, mouseY; // last time mouse click coordinates
int mouseButton;
int window1, window2; // two glut window
bool isMouseMoving = false;
bool isSelectMode = false;
double horizontalAngle = 0;
double verticalAngle = 0;
double viewDist;

PlyCloud * pointCloud1 = new PlyCloud();		// point cloud class
PlyCloud * pointCloud2 = new PlyCloud();

void displayInit(void); // display initialize
// initialize light
void initLight(void);
// draw point cloud using OPENGL
void drawPointCloud(PlyCloud * pointCloud);

/*
displayInit, initialize the display call back function
*/
void displayInit(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, (double)WIDTH / HEIGHT, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

// initial light
void initLight(void)
{
	GLfloat lightPos[] = { 0.0, 0.0, 1.0, 1.0 };
	GLfloat lightColor[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

// select point - only called when left button of the mouse is pressed
void selectPoint(PlyCloud * pointCloud)
{
	GLdouble matModelView[16], matProjection[16]; // model view matrix and projection matrix
	GLint viewPort[4];  // view port vector
	GLdouble mousePosX = (double)mouseX;  // mouse position
	GLdouble mousePosY = (double)mouseY;

	GLdouble objNearX, objNearY, objNearZ;
	GLdouble objFarX, objFarY, objFarZ;
	// get the matrices
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetIntegerv(GL_VIEWPORT, viewPort);

	// get the 3D coordinates of the position of mouse click on both near plane and far plane, so that we know the ray vector
	gluUnProject(mousePosX, mousePosY, 0.0, matModelView, matProjection, viewPort, &objNearX, &objNearY, &objNearZ);
	gluUnProject(mousePosX, mousePosY, 1.0, matModelView, matProjection, viewPort, &objFarX, &objFarY, &objFarZ);

	glm::dvec3 objNearPt = glm::dvec3(objNearX, objNearY, objNearZ); // 3D coordinates of near plane
	glm::dvec3 objFarPt = glm::dvec3(objFarX, objFarY, objFarZ); // 3D coordinates of far plan
	glm::dvec3 rayVector = objFarPt - objNearPt;
	// std::cout << rayVector.x << " " << rayVector.y << " " << rayVector.z << std::endl;
	// system("PAUSE");
	// then we should do the collision detect
	// iteration every point
	std::vector<CPoint> vertexList = pointCloud->get_vertex_list();
	std::vector<int> vertexOnTheRay;
	for (size_t i = 0; i < vertexList.size(); i++)
	{
		CPoint vert = vertexList[i];
		glm::dvec3 vertVec = glm::dvec3(vert[0], vert[1], vert[2]);
		glm::dvec3 vertVecFromStart = vertVec - objNearPt;
		// std::cout << vertVecFromStart.x << " " << vertVecFromStart.y << " " << vertVecFromStart.z << std::endl;
		// judge whether the vertex is on the ray described by rayVector
		// see the inner product. if zero, yes
		double dotProduct = glm::dot(vertVecFromStart, rayVector);
		double angle = dotProduct / (glm::l2Norm(vertVecFromStart) * glm::l2Norm(rayVector));
		std::cout << "angle " << acos(angle) << std::endl;
		if (abs(acos(angle) - 0.0) < 0.03)  // to avoid the error produced by double type
		{
			std::cout << "Hit points " << i << std::endl;
			system("PAUSE");
			vertexOnTheRay.push_back(i);
		}
		// std::cout << "233 " << i << std::endl;
	}
}

// draw point cloud
void drawPointCloud(PlyCloud * pointCloud)
{
	std::vector<CPoint> vertexList = pointCloud->get_vertex_list();
	std::vector<CPoint> normalList = pointCloud->get_normal_list();

	for (size_t i = 0; i < vertexList.size(); i++)
	{
		CPoint vert = vertexList[i];
		CPoint norl = normalList[i];

		glPointSize(2.5);
		glBegin(GL_POINTS);
		glColor3d(1.0, 1.0, 1.0);
		glVertex3d(vert[0], vert[1], vert[2]);
		glNormal3d(norl[0], norl[1], norl[2]);
		glEnd();
	}
}