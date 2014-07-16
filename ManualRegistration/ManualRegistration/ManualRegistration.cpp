/*
 * ManualRegistration used for manual align the point cloud got from the 3D scanner
 * Author : Jerome Jiang
 * Date : 7/8/2014
 */

#include "stdafx.h"

#include "CallBackFunc1.hpp"
#include "CallBackFunc2.hpp"

// call back functions





int main(int argc, char** argv)
{
	if (argc == 3)
	{
		filename1 = argv[1];
		filename2 = argv[2];
	}
	else
	{
		std::cout << "Error Input..." << std::endl;
		exit(0);
	}

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 100);
	// first window
	window1 = glutCreateWindow("Point Cloud 1"); // window1 = 1 if the window is created successfully
	initLight();
	glutDisplayFunc(display1);
	glutMouseFunc(mouseClick1);
	glutMotionFunc(mouseMove1);
	glutKeyboardFunc(keyboard);

	glutInitWindowPosition(700, 100);
	// second window
	window2 = glutCreateWindow("Point Cloud 2");
	initLight();
	glutDisplayFunc(display2);
	glutMouseFunc(mouseClick2);
	glutMotionFunc(mouseMove2);
	glutKeyboardFunc(keyboard);

	glutMainLoop();

	return 0;
}


