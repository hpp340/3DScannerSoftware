#include "interactiveMeshViewer.h"

interactiveMeshViewer::interactiveMeshViewer()
{
	isSelectionMode = false;
	meshID = 0;
}

interactiveMeshViewer::interactiveMeshViewer(int numMesh)
{
	isSelectionMode = false;
	meshID = numMesh;
}

interactiveMeshViewer::~interactiveMeshViewer()
{
}

void interactiveMeshViewer::mousePressEvent(QMouseEvent * mouseEvent)
{
	mouseButton = mouseEvent->button();
	latestMousePos = mouseEvent->pos();
	isLatestMouseOK = arcball(latestMousePos, latestMouse3DPos);

	// debug output
	if (mouseButton == Qt::LeftButton)
	{
		std::cout << "Mouse Left Press..." << std::endl;
	}
	else if (mouseButton == Qt::RightButton)
	{
		std::cout << "Mouse Right Press..." << std::endl;
	}
	// entered selection mode
	if (isSelectionMode)
	{
		std::cout << "selecting..." << std::endl;
		int selectedVertexId = selectVertex();
		std::cout << selectedVertexId << std::endl;
		drawSelectedVertex(selectedVertexId);
	}
}

int interactiveMeshViewer::selectVertex()
{
	GLdouble mousePosX = latestMousePos.x();
	GLdouble mousePosY = height() - latestMousePos.y();

	GLdouble objNearX, objNearY, objNearZ;
	GLdouble objFarX, objFarY, objFarZ;

	// get the 3D coordinates of the position of mouse click on both near plane and far plane, so that we know the ray vector
	gluUnProject(mousePosX, mousePosY, 0.0, matModelView, matProjection, viewPort, &objNearX, &objNearY, &objNearZ);
	gluUnProject(mousePosX, mousePosY, 1.0, matModelView, matProjection, viewPort, &objFarX, &objFarY, &objFarZ);

	glm::dvec3 objNearPt = glm::dvec3(objNearX, objNearY, objNearZ); // 3D coordinates of near plane
	glm::dvec3 objFarPt = glm::dvec3(objFarX, objFarY, objFarZ); // 3D coordinates of far plan
	glm::dvec3 rayVector = objFarPt - objNearPt;

	// then we should do the collision detect
	// iteration every point
	std::vector<JVertex *> vertexList = pointCloud->getJVertexList();
	std::vector<int> vertexOnTheRay;
	double minAngle = PI;
	int minVertexId = 0;
	for (size_t i = 0; i < vertexList.size(); i++)
	{
		CPoint vert = vertexList[i]->getPoint();
		glm::dvec3 vertVec = glm::dvec3(vert[0], vert[1], vert[2]);
		glm::dvec3 vertVecFromStart = vertVec - objNearPt;
		// judge whether the vertex is on the ray described by rayVector
		// see the inner product. if zero, yes
		double dotProduct = glm::dot(vertVecFromStart, rayVector);
		double angle = dotProduct / (glm::l2Norm(vertVecFromStart) * glm::l2Norm(rayVector));
		//std::cout << "angle " << acos(angle) << std::endl;
		if (abs(acos(angle) - 0.0) < 0.03)  // to avoid the error produced by double type
		{
			std::cout << "Hit points " << i << std::endl;
			//system("PAUSE");
			vertexOnTheRay.push_back((int)i);
		}
		if (abs(acos(angle)) < minAngle)
		{
			minVertexId = (int)i;
			minAngle = abs(acos(angle));
		}
	}
	std::vector<int>::iterator findResult = std::find(allSelectedVertex.begin(), allSelectedVertex.end(), minVertexId);
	if (findResult != allSelectedVertex.end()) // this vertex has been selected
	{
		allSelectedVertex.erase(findResult);
	}
	else
	{
		allSelectedVertex.push_back(minVertexId);
	}
	updateGL();
	return minVertexId;
}

void interactiveMeshViewer::drawSelectedVertex(int selectedId)
{
	makeCurrent();
	std::cout << "painting selected vertex " << selectedId << std::endl;
	JVertex * selectedVertex = pointCloud->get_vertex(selectedId);
	CPoint selectedPos = selectedVertex->getPoint();
	CPoint selectedNormal = selectedVertex->getNormal();
	glPointSize(20);
	glColor3d(1.0, 0.5, 0.0);
	glBegin(GL_POINTS);
	glVertex3d(selectedPos[0], selectedPos[1], selectedPos[2]);
	if (selectedVertex->hasNormal())
	{
		glNormal3d(selectedNormal[0], selectedNormal[1], selectedNormal[2]);
	}
	glEnd();
}

// override the original method to draw selected vertex
void interactiveMeshViewer::drawMesh()
{
	std::cout << "interactiveMeshViewer:drawMesh" << std::endl;
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
	std::vector<JVertex *> vertexList = pointCloud->getJVertexList();
	std::vector<bool> deletedVertexList = pointCloud->get_deleted_vertex_list();
	std::cout << "interactiveMeshViewer:vertex list size " << vertexList.size() << std::endl;
	for (size_t i = 0; i < vertexList.size(); i++)
	{
		if (! deletedVertexList[i])
		{
			JVertex * jVert = vertexList[i];
			CPoint vert = jVert->getPoint();

			if (std::find(allSelectedVertex.begin(), allSelectedVertex.end(), i) != allSelectedVertex.end()) // this vertex is selected
			{
				glPointSize(15);
				glColor3d(1.0, 0.5, 0.0);
				glBegin(GL_POINTS);
				glVertex3d(vert[0], vert[1], vert[2]);
				if (jVert->hasNormal())
				{
					CPoint norl = jVert->getNormal();
					glNormal3d(norl[0], norl[1], norl[2]);
				}
				glEnd();
			}
			else
			{
				glPointSize(10);
				if ((meshID == 0) || (meshID == 1))
				{
					glColor3d(0.1, 0.5, 0.8);
				}
				else
				{
					glColor3d(0.7, 0.55, 0.55);
				}
				glBegin(GL_POINTS);
				glVertex3d(vert[0], vert[1], vert[2]);
				if (jVert->hasNormal())
				{
					CPoint norl = jVert->getNormal();
					glNormal3d(norl[0], norl[1], norl[2]);
				}
				glEnd();
			}
		}
	}
}

// slots
void interactiveMeshViewer::enterSelectionMode()
{
	if (!isSelectionMode)
	{
		std::cout << "MeshViewer: Enter Selection Mode" << std::endl;
		isSelectionMode = true;
	}
}

void interactiveMeshViewer::quitSelectionMode()
{
	if (isSelectionMode)
	{
		std::cout << "MeshViewer: Leaving Selection Mode" << std::endl;
		isSelectionMode = false;
	}
}