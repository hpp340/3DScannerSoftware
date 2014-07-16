#pragma once

#include "CommonFuncsAndVariables.hpp"

void display2(void);
void mouseClick2(int, int, int, int);

/*
display call back function used for the second window
*/
void display2()
{
	pointCloud2->read_ply(filename2);

	displayInit();
	// to calculate the up vector used for gluLookAt
	glm::vec3 viewPoint = glm::vec3(1.0 * sin(horizontalAngle), 1.0 * sin(verticalAngle), 1.0*cos(horizontalAngle));
	glm::vec3 rightVec = glm::vec3(-cos(horizontalAngle), 0.0, 1.0*sin(horizontalAngle));
	glm::vec3 upVector = glm::cross(-viewPoint, rightVec);

	viewDist = 2.0;
	gluLookAt(viewDist*sin(horizontalAngle), viewDist*sin(verticalAngle), viewDist*cos(horizontalAngle), 0.0, 0.0, 0.0, upVector.x, upVector.y, upVector.z);
	glPushMatrix();
	// draw point cloud
	drawPointCloud(pointCloud2);

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

// call back function for mouse click event
void mouseClick2(int button, int state, int x, int y)
{

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		std::cout << "left click..." << std::endl;
		mouseX = x;
		mouseY = HEIGHT - y - 1;
		mouseButton = button;
		if ((!isMouseMoving) && (isSelectMode))
		{
			selectPoint(pointCloud2);
		}
	}
}