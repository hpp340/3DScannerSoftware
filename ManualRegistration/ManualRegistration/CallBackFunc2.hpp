/*
This hpp file includes Call back functions used by the second window
*/

#pragma once

#include "CommonFuncsAndVariables.hpp"

void display2(void);
void mouseClick2(int, int, int, int);
void mouseMove2(int, int);

/*
display call back function used for the second window
*/
void display2()
{
	pointCloud2->read_ply(filename2);

	displayInit();
	// to calculate the up vector used for gluLookAt
	glm::vec3 viewPoint = glm::vec3(1.0 * sin(horizontalAngle2), 1.0 * sin(verticalAngle2), 1.0*cos(horizontalAngle2));
	glm::vec3 rightVec = glm::vec3(-cos(horizontalAngle2), 0.0, 1.0*sin(horizontalAngle2));
	glm::vec3 upVector = glm::cross(-viewPoint, rightVec);

	gluLookAt(viewDist*sin(horizontalAngle2), viewDist*sin(verticalAngle2), viewDist*cos(horizontalAngle2), 0.0, 0.0, 0.0, upVector.x, upVector.y, upVector.z);
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
		if ((!isMouseMoving2) && (isSelectMode2))
		{
			selectPoint(pointCloud2);
		}
	}
}

// call back function for mouse move event - dragging
void mouseMove2(int x, int y)
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
		if ((movingDistX > MOVING_THRES) || (movingDistY > MOVING_THRES))
		{
			isMouseMoving2 = true;
		}
		else
		{
			isMouseMoving2 = false;
		}
		if (movingDistX > MOVING_THRES)
		{
			if (mouseX < oldX)
			{
				std::cout << "moving left..." << std::endl;
				horizontalAngle2 += 0.2;
			}
			else
			{
				std::cout << "moving right..." << std::endl;
				horizontalAngle2 -= 0.2;
			}
		}
		if (movingDistY > MOVING_THRES)
		{
			if (mouseY < oldY)
			{
				std::cout << "moving down..." << std::endl;
				verticalAngle2 += 0.2;
			}
			else
			{
				std::cout << "moving up..." << std::endl;
				verticalAngle2 -= 0.2;
			}
		}
		std::cout << "left click moving.." << std::endl;
	}
	glutPostRedisplay();
}