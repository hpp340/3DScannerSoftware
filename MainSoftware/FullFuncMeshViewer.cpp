#include "FullFuncMeshViewer.h"


FullFuncMeshViewer::FullFuncMeshViewer()
{
	isSelectionMode = false;
}


FullFuncMeshViewer::~FullFuncMeshViewer()
{
}

void FullFuncMeshViewer::mouseMoveEvent(QMouseEvent * mouseEvent)
{
	QPoint newMousePos = mouseEvent->pos();

	makeCurrent();
	if (isLatestMouseOK)
	{
		switch (mouseButton)
		{
		case Qt::LeftButton:
			std::cout << "FullFunc:Mouse Left Moving..." << std::endl;
			if (isSelectionMode)
			{
				selectGroupVertex(newMousePos);
			}
			else
			{
				rotationView(newMousePos);
			}
			break;
		case Qt::RightButton:
			std::cout << "FullFunc:Mouse Right Moving..." << std::endl;
			if (! isSelectionMode)
			{
				translateView(newMousePos);
			}
			break;
		default:
			break;
		}
	}

	latestMousePos = newMousePos;
	isLatestMouseOK = arcball(latestMousePos, latestMouse3DPos);
	updateGL();
}

void FullFuncMeshViewer::selectGroupVertex(QPoint newPos)
{

	glm::dvec3 newNear, newFar;
	glm::dvec3 newRay = getRayVector(newPos, newNear, newFar);
	QPoint topRight(latestMousePos.x(), newPos.y());

	// then we should do the collision detect
	// iteration every point
	std::vector<JVertex *> vertexList = pointCloud->getJVertexList();
	for (size_t i = 0; i < vertexList.size(); i++)
	{
		JVertex * currVertex = vertexList[i];
		CPoint currPoint = currVertex->getPoint();
		glm::dvec3 vertVec = glm::dvec3(currPoint[0], currPoint[1], currPoint[2]);
		glm::dvec3 vertVecFromStart = vertVec - newNear;
		// judge whether the vertex is on the ray described by rayVector
		// see the inner product. if zero, yes
		double dotProduct = glm::dot(vertVecFromStart, newRay);
		double angle = dotProduct / (glm::l2Norm(vertVecFromStart) * glm::l2Norm(newRay));
		//std::cout << "angle " << acos(angle) << std::endl;
		if (abs(acos(angle) - 0.0) < SELECT_MIN_DIST)  // to avoid the error produced by double type
		{
			std::cout << "Hit points " << i << std::endl;
			//system("PAUSE");
			selectedVertices.push_back((int)i);
			std::vector<JFace *> faceList = currVertex->getFacesList();
			if (pointCloud->hasFace())
			{
				for (size_t i = 0; i < faceList.size(); i++)
				{
					int _faceId = faceList[i]->faceId;
					std::cout << "selected faces " << _faceId << std::endl;
					selectedFaces.push_back(_faceId);
				}
			}
		}
	}
}

glm::dvec3 FullFuncMeshViewer::getRayVector(QPoint point, glm::dvec3 & nearPt, glm::dvec3 & farPt)
{
	GLdouble mousePosX = point.x();
	GLdouble mousePosY = height() - point.y();

	GLdouble objNearX, objNearY, objNearZ;
	GLdouble objFarX, objFarY, objFarZ;

	// get the 3D coordinates of the position of mouse click on both near plane and far plane, so that we know the ray vector
	gluUnProject(mousePosX, mousePosY, 0.0, matModelView, matProjection, viewPort, &objNearX, &objNearY, &objNearZ);
	gluUnProject(mousePosX, mousePosY, 1.0, matModelView, matProjection, viewPort, &objFarX, &objFarY, &objFarZ);

	nearPt = glm::dvec3(objNearX, objNearY, objNearZ); // 3D coordinates of near plane
	farPt = glm::dvec3(objFarX, objFarY, objFarZ); // 3D coordinates of far plan
	glm::dvec3 rayVector = farPt - nearPt;

	return rayVector;
}

void FullFuncMeshViewer::drawMeshPoints()
{
	std::vector<JVertex*> jVertexList = pointCloud->getJVertexList();

	std::vector<bool> deletedVertexList = pointCloud->get_deleted_vertex_list();

	for (size_t i = 0; i < jVertexList.size(); i++)
	{
		if (!deletedVertexList[i])
		{
			CPoint vert = jVertexList[i]->getPoint();
			if (jVertexList[i]->hasNormal())
			{
				CPoint norl = jVertexList[i]->getNormal();
				if (std::find(selectedVertices.begin(), selectedVertices.end(), i) == selectedVertices.end())
				{
					glPointSize(10);
					glColor3d(0.1, 0.5, 0.8);
					glBegin(GL_POINTS);
					glVertex3d(vert[0], vert[1], vert[2]);
					glNormal3d(norl[0], norl[1], norl[2]);
					glEnd();
				}
				else
				{
					glPointSize(10);
					glColor3d(1.0, 0.5, 0.0);
					glBegin(GL_POINTS);
					glVertex3d(vert[0], vert[1], vert[2]);
					glNormal3d(norl[0], norl[1], norl[2]);
					glEnd();
				}
			}
			else
			{
				if (std::find(selectedVertices.begin(), selectedVertices.end(), i) == selectedVertices.end())
				{
					glPointSize(10);
					glColor3d(0.1, 0.5, 0.8);
					glBegin(GL_POINTS);
					glVertex3d(vert[0], vert[1], vert[2]);
					glEnd();
				}
				else
				{
					glPointSize(10);
					glColor3d(1.0, 0.5, 0.0);
					glBegin(GL_POINTS);
					glVertex3d(vert[0], vert[1], vert[2]);
					glEnd();
				}
			}
		}
	}
}

void FullFuncMeshViewer::drawMeshWireframe()
{
	std::cout << "drawMeshWireframe" << std::endl;
	std::vector<JFace *> faceList = pointCloud->get_face_list();
	std::vector<JVertex *> vertexList = pointCloud->getJVertexList();
	std::vector<bool> delectedFaceList = pointCloud->get_deleted_face_list();
	std::cout << faceList.size() << " " << vertexList.size() << std::endl;
	std::cout << delectedFaceList.size() << std::endl;
	glLineWidth(2.5);
	for (size_t i = 0; i < faceList.size(); i++)
	{
		JFace *faceIter = faceList[i];
		//std::cout << i << std::endl;
		if (! delectedFaceList[faceIter->faceId])
		{
			CPoint v1 = vertexList[faceIter->vert1Id]->getPoint();
			CPoint v2 = vertexList[faceIter->vert2Id]->getPoint();
			CPoint v3 = vertexList[faceIter->vert3Id]->getPoint();
			if (std::find(selectedFaces.begin(), selectedFaces.end(), faceIter->faceId) != selectedFaces.end())
			{
				glColor3d(1.0, 0.5, 0.0);
				glBegin(GL_POLYGON);
				glVertex3d(v1[0], v1[1], v1[2]);
				glVertex3d(v2[0], v2[1], v2[2]);
				glVertex3d(v3[0], v3[1], v3[2]);
				glEnd();
			}
			else
			{
				glColor3d(1.0, 1.0, 1.0);
				glBegin(GL_POLYGON);
				glVertex3d(v1[0], v1[1], v1[2]);
				glVertex3d(v2[0], v2[1], v2[2]);
				glVertex3d(v3[0], v3[1], v3[2]);
				glEnd();
			}
		}
		
	}
}

void FullFuncMeshViewer::drawMeshFlat()
{
	std::vector<JFace*> faceList = pointCloud->get_face_list();
	std::vector<JVertex *> jVertexList = pointCloud->getJVertexList();
	std::vector<bool> delectedFaceList = pointCloud->get_deleted_face_list();

	for (size_t i = 0; i < faceList.size(); i++)
	{
		JFace *faceIter = faceList[i];
		if (!delectedFaceList[faceIter->faceId])
		{
			CPoint v1 = jVertexList[faceIter->vert1Id]->getPoint();
			CPoint v2 = jVertexList[faceIter->vert2Id]->getPoint();
			CPoint v3 = jVertexList[faceIter->vert3Id]->getPoint();
			CPoint faceNormal = faceIter->getFaceNormal();
			if (std::find(selectedFaces.begin(), selectedFaces.end(), faceIter->faceId) != selectedFaces.end()) // selected faces
			{
				glColor3d(1.0, 0.5, 0.0);
				glBegin(GL_TRIANGLES);
				glNormal3d(faceNormal[0], faceNormal[1], faceNormal[2]);
				glVertex3d(v1[0], v1[1], v1[2]);
				glVertex3d(v2[0], v2[1], v2[2]);
				glVertex3d(v3[0], v3[1], v3[2]);
				glEnd();
			}
			else // unselected faces
			{
				glColor3d(0.1, 0.5, 0.8);
				glBegin(GL_TRIANGLES);
				glNormal3d(faceNormal[0], faceNormal[1], faceNormal[2]);
				if (pointCloud->hasTexture())
				{
					CPoint2 text1 = jVertexList[faceIter->vert1Id]->getTexture();
					CPoint2 text2 = jVertexList[faceIter->vert2Id]->getTexture();
					CPoint2 text3 = jVertexList[faceIter->vert3Id]->getTexture();
					glTexCoord2d(text1[0], text1[1]);
					glVertex3d(v1[0], v1[1], v1[2]);
					glTexCoord2d(text2[0], text2[1]);
					glVertex3d(v2[0], v2[1], v2[2]);
					glTexCoord2d(text3[0], text3[1]);
					glVertex3d(v3[0], v3[1], v3[2]);

				}
				else
				{
					glVertex3d(v1[0], v1[1], v1[2]);
					glVertex3d(v2[0], v2[1], v2[2]);
					glVertex3d(v3[0], v3[1], v3[2]);
				}
				glEnd();
			}
		}
	}
}

void FullFuncMeshViewer::drawMeshSmooth()
{
	std::vector<JFace*> faceList = pointCloud->get_face_list();
	std::vector<JVertex *> jVertexList = pointCloud->getJVertexList();
	std::vector<bool> delectedFaceList = pointCloud->get_deleted_face_list();

	for (size_t i = 0; i < faceList.size(); i++)
	{
		JFace *faceIter = faceList[i];

		if (!delectedFaceList[faceIter->faceId])
		{
			CPoint v1 = jVertexList[faceIter->vert1Id]->getPoint();
			CPoint v2 = jVertexList[faceIter->vert2Id]->getPoint();
			CPoint v3 = jVertexList[faceIter->vert3Id]->getPoint();

			CPoint vert1Normal = jVertexList[faceIter->vert1Id]->getNormal();
			CPoint vert2Normal = jVertexList[faceIter->vert2Id]->getNormal();
			CPoint vert3Normal = jVertexList[faceIter->vert3Id]->getNormal();

			if (std::find(selectedFaces.begin(), selectedFaces.end(), faceIter->faceId) != selectedFaces.end()) // selected faces
			{
				glColor3d(1.0, 0.5, 0.0);
				glBegin(GL_TRIANGLES);
				glNormal3d(vert1Normal[0], vert1Normal[1], vert1Normal[2]);
				glVertex3d(v1[0], v1[1], v1[2]);
				glNormal3d(vert2Normal[0], vert2Normal[1], vert2Normal[2]);
				glVertex3d(v2[0], v2[1], v2[2]);
				glNormal3d(vert3Normal[0], vert3Normal[1], vert3Normal[2]);
				glVertex3d(v3[0], v3[1], v3[2]);
				glEnd();
			}
			else // unselected faces
			{
				glColor3d(0.1, 0.5, 0.8);
				glBegin(GL_TRIANGLES);
				if (pointCloud->hasTexture())
				{
					CPoint2 text1 = jVertexList[faceIter->vert1Id]->getTexture();
					CPoint2 text2 = jVertexList[faceIter->vert2Id]->getTexture();
					CPoint2 text3 = jVertexList[faceIter->vert3Id]->getTexture();

					glNormal3d(vert1Normal[0], vert1Normal[1], vert1Normal[2]);
					glTexCoord2d(text1[0], text1[1]);
					glVertex3d(v1[0], v1[1], v1[2]);

					glNormal3d(vert2Normal[0], vert2Normal[1], vert2Normal[2]);
					glTexCoord2d(text2[0], text2[1]);
					glVertex3d(v2[0], v2[1], v2[2]);

					glNormal3d(vert3Normal[0], vert3Normal[1], vert3Normal[2]);
					glTexCoord2d(text3[0], text3[1]);
					glVertex3d(v3[0], v3[1], v3[2]);
				}
				else
				{
					glNormal3d(vert1Normal[0], vert1Normal[1], vert1Normal[2]);
					glVertex3d(v1[0], v1[1], v1[2]);
					glNormal3d(vert2Normal[0], vert2Normal[1], vert2Normal[2]);
					glVertex3d(v2[0], v2[1], v2[2]);
					glNormal3d(vert3Normal[0], vert3Normal[1], vert3Normal[2]);
					glVertex3d(v3[0], v3[1], v3[2]);
				}
				glEnd();
			}
		}
	}
}

// slots
void FullFuncMeshViewer::enterSelectionMode()
{
	isSelectionMode = true;
}

void FullFuncMeshViewer::quitSelectionMode()
{
	isSelectionMode = false;
}

void FullFuncMeshViewer::clearSelected()
{
	isSelectionMode = false;
	selectedVertices.clear();
	selectedFaces.clear();
	updateGL();
}

void FullFuncMeshViewer::deleteSelected()
{
	for (size_t i = 0; i < selectedVertices.size(); i++)
	{
		int v = (int)selectedVertices[i];
		pointCloud->add_deleted_vertex(v);
	}

	for (size_t i = 0; i < selectedFaces.size(); i++)
	{
		int f = selectedFaces[i];
		pointCloud->add_deleted_face(f);
	}
	updateGL();
}
