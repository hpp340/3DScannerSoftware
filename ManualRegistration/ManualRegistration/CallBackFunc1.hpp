/*
This hpp file is used as the callback functions for the first window
*/

#pragma once

#include "CommonFuncsAndVariables.hpp"

// call back functions
void mouseClick1(int, int, int, int);
void display1(void);
void mouseMove1(void);

/*
display - call back function
*/
void display1()
{
<<<<<<< Updated upstream
	pointCloud1->read_ply("testx.ply");
=======
	pointCloud1->read_ply(filename1);
>>>>>>> Stashed changes

	displayInit();
	// to calculate the up vector used for gluLookAt
	glm::vec3 viewPoint = glm::vec3(1.0 * sin(horizontalAngle1), 1.0 * sin(verticalAngle1), 1.0*cos(horizontalAngle1));
	glm::vec3 rightVec = glm::vec3(-cos(horizontalAngle1), 0.0, 1.0*sin(horizontalAngle1));
	glm::vec3 upVector = glm::cross(-viewPoint, rightVec);

	gluLookAt(viewDist*sin(horizontalAngle1), viewDist*sin(verticalAngle1), viewDist*cos(horizontalAngle1), 0.0, 0.0, 0.0, upVector.x, upVector.y, upVector.z);
	glPushMatrix();
	// draw point cloud
	drawPointCloud(pointCloud1);

	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}

// call back function for mouse click event
void mouseClick1(int button, int state, int x, int y)
{

	if ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN))
	{
		std::cout << "left click..." << std::endl;
		mouseX = x;
		mouseY = HEIGHT - y - 1;
		mouseButton = button;
		if ((!isMouseMoving1) && (isSelectMode1))
		{
			selectPoint(pointCloud1);
		}
	}
	//else if ((button == ) && (state == GLUT_DOWN)) // button=3 means scroll up
	//{
	//	std::cout << "Scrolling up..." << std::endl;
	//}
}

// call back function for mouse move event - dragging
void mouseMove1(int x, int y)
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
			isMouseMoving1 = true;
		}
		else
		{
			isMouseMoving1 = false;
		}
		if (movingDistX > MOVING_THRES)
		{
			if (mouseX < oldX)
			{
				std::cout << "moving left..." << std::endl;
				horizontalAngle1 += 0.2;
			}
			else
			{
				std::cout << "moving right..." << std::endl;
				horizontalAngle1 -= 0.2;
			}
		}
		if (movingDistY > MOVING_THRES)
		{
			if (mouseY < oldY)
			{
				std::cout << "moving down..." << std::endl;
				verticalAngle1 += 0.2;
			}
			else
			{
				std::cout << "moving up..." << std::endl;
				verticalAngle1 -= 0.2;
			}
		}
		std::cout << "left click moving.." << std::endl;
	}
	glutPostRedisplay();
}