/*
This hpp file includes all global variables and common functions used by the two windows
Author:Jerome Jiang
*/

#pragma once

#include <stdlib.h>
#include "header\glut.h"
//#include "header\openglut.h"
#include "header\glm\glm.hpp"
#include "header\glm\gtx\norm.hpp"
#include "header\PlyCloud.h"
#include "header\Point.h"

#include "stdafx.h"
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <vector>

#define WIDTH 600	// width of glut window
#define HEIGHT 600	// height of glut window
#define PI 3.14159265	// pi
#define FOVY 80		// view angle used for gluLookAt
#define ZNEAR 1.0	// z near plane of view field
#define ZFAR 5.0	// z far plane of view field
#define MOVING_THRES 6 // if the mouse button is held and moved more than MOVING_THRES pixes, we regard it as a motion
#define ZOOM_DIFF 0.2

const char *filename1, *filename2;
int mouseX, mouseY; // last time mouse click coordinates

int mouseButton;

int window1, window2; // two glut window
bool isMouseMoving1 = false;	// judge if the mouse button is held and moving
bool isMouseMoving2 = false;
bool isSelectMode1 = false;		// judge if we should enter selection mode, control by keyboard
bool isSelectMode2 = false;
double horizontalAngle1 = 0.0;
double horizontalAngle2 = 0.0;
double verticalAngle1 = 0.0;
double verticalAngle2 = 0.0;
double viewDist = 2.0;
std::vector<int> selectedPts1, selectedPts2;

PlyCloud * pointCloud1 = new PlyCloud();		// point cloud class
PlyCloud * pointCloud2 = new PlyCloud();

void displayInit(void); // display initialize
// initialize light
void initLight(void);
// draw point cloud using OPENGL
void drawPointCloud(PlyCloud * pointCloud);
// select point function
void selectPoint(PlyCloud * pointCloud);

// keyboard function
// this is a call back function used by two window
// using glutGetWindow() to distinguish the two window
void keyboard(unsigned char, int, int);

// call back function of mouse scroll
// void scroll(void);

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
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
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

	// judge which point cloud the selected points belong to
	if (glutGetWindow() != 0)
	{
		if (glutGetWindow() == 1)
		{
			std::cout << "Hit in Window 1" << std::endl;
			selectedPts1 = vertexOnTheRay;	// this is a hard copy
		}
		else if (glutGetWindow() == 2)
		{
			std::cout << "Hit in Window 2" << std::endl;
			selectedPts2 = vertexOnTheRay;
		}
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

// call back function - keyboard
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's': // enter select mode
		if (glutGetWindow() == 1)
		{
			std::cout << "Window 1 Enter Selection Mode..." << std::endl;
			isSelectMode1 = true;
		}
		else if (glutGetWindow() == 2)
		{
			std::cout << "Window 2 Enter Selection Mode..." << std::endl;
			isSelectMode2 = true;
		}
		break;

	case 'u': // quit select mode
		if (glutGetWindow() == 1)
		{
			std::cout << "Window 1 Quit Selection Mode..." << std::endl;
			isSelectMode1 = false;
		}
		else if (glutGetWindow() == 2)
		{
			std::cout << "Window 2 Quit Selection Mode..." << std::endl;
			isSelectMode2 = false;
		}
		break;

	case 'i': // zoom in
		if (glutGetWindow() == 1)
		{
			std::cout << "Zoom In Window 1..." << std::endl;
			viewDist += ZOOM_DIFF;
		}
		else if (glutGetWindow() == 2)
		{
			std::cout << "Zoom In Window 1..." << std::endl;
			viewDist += ZOOM_DIFF;
		}
		glutPostRedisplay();
		break;
		
	case 'o': // zoom out
		if (glutGetWindow() == 1)
		{
			std::cout << "Zoom Out Window 1..." << std::endl;
			viewDist -= ZOOM_DIFF;
		}
		else if (glutGetWindow() == 2)
		{
			std::cout << "Zoom Out Window 1..." << std::endl;
			viewDist -= ZOOM_DIFF;
		}
		glutPostRedisplay();
		break;
	default:
		break;
	}
}

// call back function - mouse scroll
//void scroll(void)
//{
//
//}