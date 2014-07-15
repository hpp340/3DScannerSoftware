/*
 * ManualRegistration used for manual align the point cloud got from the 3D scanner
 * Author : Jerome Jiang
 * Date : 7/8/2014
 */

#include "stdafx.h"
#include <Windows.h>
#include <gl\gl.h>
#include <gl\glu.h>
#include <vector>

#include "header\glut.h"
#include "header\glm\glm.hpp"
#include "header\glm\gtx\norm.hpp"
#include "header\PlyCloud.h"
#include "header\Point.h"

#define WIDTH 1280	// width of glut window
#define HEIGHT 800	// height of glut window
#define PI 3.14159265	// pi
#define FOVY 80		// view angle used for gluLookAt
#define ZNEAR 1.0	// z near plane of view field
#define ZFAR 5.0	// z far plane of view field

const char * filename;
int mouseX, mouseY; // last time mouse click coordinates
int mouseButton;
bool isMouseMoving = false;
bool isSelectMode = false;
double horizontalAngle = 0;
double verticalAngle = 0;
double viewDist;

PlyCloud * pointCloud = new PlyCloud();		// point cloud class

// call back functions
void display(void);
void drawPointCloud(void);
void mouseClick(int, int, int, int);
void mouseMove(int, int);
void initLight(void);
void selectPoint(void);
void keyboard(unsigned char, int, int);

int main(int argc, char** argv)
{
	if (argc == 2)
	{
		filename = argv[1];
	}
	else
	{
		std::cout << "Error Input..." << std::endl;
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("HelloWorld!");
	initLight();
	glutDisplayFunc(display);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}

/*
 display - call back function
*/
void display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(FOVY, (double)WIDTH/HEIGHT, ZNEAR, ZFAR);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// to calculate the up vector used for gluLookAt
	glm::vec3 viewPoint = glm::vec3(1.0 * sin(horizontalAngle), 1.0 * sin(verticalAngle), 1.0*cos(horizontalAngle));
	glm::vec3 rightVec = glm::vec3(-cos(horizontalAngle), 0.0, 1.0*sin(horizontalAngle));
	glm::vec3 upVector = glm::cross(-viewPoint, rightVec);

	viewDist = 2.0;
	gluLookAt(viewDist*sin(horizontalAngle), viewDist*sin(verticalAngle), viewDist*cos(horizontalAngle), 0.0, 0.0, 0.0, upVector.x, upVector.y, upVector.z);
	glPushMatrix();
	// draw point cloud
	drawPointCloud();

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

// select point - only called when left button of the mouse is pressed
void selectPoint(void)
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

// call back function for mouse click event
void mouseClick(int button, int state, int x, int y)
{

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		std::cout << "left click..." << std::endl;
		mouseX = x;
		mouseY = HEIGHT - y - 1;
		mouseButton = button;
		if ((!isMouseMoving) && (isSelectMode))
		{
			selectPoint();
		}
	}
}

// call back function for mouse move event - dragging
void mouseMove(int x, int y)
{
	int oldX = mouseX;
	int oldY = mouseY;

	mouseX = x;
	mouseY = HEIGHT - y - 1;
	if (mouseButton == GLUT_LEFT_BUTTON)
	{
		int movingDistX = abs(mouseX - oldX);
		int movingDistY = abs(mouseY - oldY);
		// to judge if the mouse is moving
		if ((movingDistX > 10) || (movingDistY > 10))
		{
			isMouseMoving = true;
		}
		else
		{
			isMouseMoving = false;
		}
		if (movingDistX > 10)
		{
			if (mouseX < oldX)
			{
				std::cout << "moving left..." << std::endl;
				horizontalAngle += 0.2;
			}
			else
			{
				std::cout << "moving right..." << std::endl;
				horizontalAngle -= 0.2;
			}
		}
		if (movingDistY > 10)
		{
			if (mouseY < oldY)
			{
				std::cout << "moving down..." << std::endl;
				verticalAngle += 0.2;
			}
			else
			{
				std::cout << "moving up..." << std::endl;
				verticalAngle -= 0.2;
			}
		}
		std::cout << "left click moving.." << std::endl;
	}
	glutPostRedisplay();
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

// draw point cloud
void drawPointCloud(void)
{
	pointCloud->read_ply(filename);
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
		std::cout << "Enter Selection Mode..." << std::endl;
		isSelectMode = true;
		break;
	case 'u': // quit select mode
		std::cout << "Quit Selection Mode..." << std::endl;
		isSelectMode = false;
		break;
	default:
		break;
	}
}
