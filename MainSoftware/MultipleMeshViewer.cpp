#include "MultipleMeshViewer.h"


MultipleMeshViewer::MultipleMeshViewer()
{
	initColorVector();
}


MultipleMeshViewer::~MultipleMeshViewer()
{
}

void MultipleMeshViewer::initColorVector()
{
	glm::vec3 color1 = glm::vec3(0.1, 0.5, 0.8);
	glm::vec3 color2 = glm::vec3(0.7, 0.55, 0.55);
	glm::vec3 color3 = glm::vec3(0.3, 0.5, 0.4);
	glm::vec3 color4 = glm::vec3(0.7, 0.7, 0.7);
	colorList.push_back(color1);
	colorList.push_back(color2);
	colorList.push_back(color3);
	colorList.push_back(color4);
}

void MultipleMeshViewer::addMesh(PlyCloud * pointCloud)
{
	meshList.push_back(pointCloud);
}

void MultipleMeshViewer::drawMesh()
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matProjection);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(matModelView);

	if (isLightOn)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}

	for (size_t i = 0; i < meshList.size(); i++)
	{
		PlyCloud * plyDraw = meshList[i];
		std::vector<CPoint> vertexList = plyDraw->get_vertex_list();
		std::vector<CPoint> normalList = plyDraw->get_normal_list();
		glm::vec3 color = colorList[i % colorList.size()];
		for (size_t j = 0; j < vertexList.size(); j++)
		{
			CPoint vert = vertexList[j];
			CPoint norl = normalList[j];
			glPointSize(10);
			glColor3d(color[0], color[1], color[2]);
			glBegin(GL_POINTS);
			glVertex3d(vert[0], vert[1], vert[2]);
			glNormal3d(norl[0], norl[1], norl[2]);
			glEnd();
		}
	}
	std::cout << "MultipleMeshViewer::drawMesh" << std::endl;
}
