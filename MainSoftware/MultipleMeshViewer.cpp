#include "MultipleMeshViewer.h"

#ifndef MIN_DIST
#define MIN_DIST 0.005
#endif

MultipleMeshViewer::MultipleMeshViewer()
{
	initColorVector();
	showMergedMesh = false;
	isMeshesMerged = false;
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
	if (meshList.size() > 1)
	{
		isMeshesMerged = false;
	}
}

void MultipleMeshViewer::paintGL()
{
	std::cout << "MultipleMeshViewer:paintGL" << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	if (isMeshLoaded)
	{
		drawAxis();
	}
	if (showMergedMesh)
	{
		drawMergedMesh();
	}
	else
	{
		drawMesh();
	}
	glPopMatrix();
}

void MultipleMeshViewer::drawMesh()
{
	std::cout << "MultipleMeshViewer::drawMesh" << std::endl;

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
		bool normalExist = false;
		if (vertexList.size() == normalList.size())
		{
			normalExist = true;
		}
		glm::vec3 color = colorList[i % colorList.size()];
		for (size_t j = 0; j < vertexList.size(); j++)
		{
			CPoint vert = vertexList[j];
			glPointSize(10);
			glColor3d(color[0], color[1], color[2]);
			glBegin(GL_POINTS);
			glVertex3d(vert[0], vert[1], vert[2]);
			if (normalExist)
			{
				CPoint norl = normalList[j];
				glNormal3d(norl[0], norl[1], norl[2]);
			}
			glEnd();
		}
	}
}

void MultipleMeshViewer::saveMesh()
{
	if (! isMeshesMerged)
	{
		QMessageBox saveFail;
		saveFail.setText("You have more than one mesh. \nCan't Save File.");
		saveFail.exec();
	}
	else
	{
		QString saveFilename = QFileDialog::getSaveFileName(this,
			tr("Save Mesh File"),
			tr("../models/"),
			tr("PLY Files (*.ply);;"
			"All Files (*)"));
		if (!saveFilename.isEmpty())
		{
			// convert QString to char *
			QByteArray byteArray = saveFilename.toUtf8();
			const char * _saveFilename = byteArray.constData();
			saveFile(_saveFilename);
		}
	}
}

void MultipleMeshViewer::saveFile(const char * filename)
{
	bool isWriteOK = mergedMesh->write_ply(filename);
	if (!isWriteOK)
	{
		QMessageBox writeFail;
		writeFail.setText("Can't Save File.");
		writeFail.exec();
	}
	std::cout << "saveFile" << std::endl;
}

void MultipleMeshViewer::mergeMeshes()
{
	std::cout << "MultipleMeshViewer::mergeMeshes" << std::endl;
	// construct kdtree according to the second mesh in the meshList
	if (meshList.size() == 2)
	{
		PlyCloud * secondMesh = meshList[1]; // second mesh
		std::vector<CPoint> secondVertexList = secondMesh->get_vertex_list();
		std::vector<CPoint> secondNormalList = secondMesh->get_normal_list();
		unsigned nv = (unsigned)secondVertexList.size();
		ANNpointArray ANNDataPts = annAllocPts(nv, 3);
		for (size_t i = 0; i < secondVertexList.size(); i++)
		{
			CPoint secondVert = secondVertexList[i];
			ANNDataPts[i][0] = secondVert[0];
			ANNDataPts[i][1] = secondVert[1];
			ANNDataPts[i][2] = secondVert[2];
		}
		std::cout << "construct kdtree" << std::endl;
		kdTree = new ANNkd_tree(ANNDataPts, nv, 3);

		PlyCloud * firstMesh = meshList[0];
		std::vector<CPoint> firstVertexList = firstMesh->get_vertex_list();
		std::vector<CPoint> firstNormalList = firstMesh->get_normal_list();

		if ((firstVertexList.size() == firstNormalList.size()) && (secondVertexList.size() == secondNormalList.size()))
		{
			std::vector<CPoint> mergedVertexList;
			std::vector<CPoint> mergedNormalList;
			std::vector<bool> secondVertexOverlapped;
			secondVertexOverlapped.assign(secondVertexList.size(), false);

			for (size_t v = 0; v < firstVertexList.size(); v++)
			{
				CPoint firstVertIter = firstVertexList[v];
				CPoint firstNormIter = firstNormalList[v];
				ANNpoint ptSearched = annAllocPt(3);
				ptSearched[0] = firstVertIter[0];
				ptSearched[1] = firstVertIter[1];
				ptSearched[2] = firstVertIter[2];
				ANNidxArray nnIdx = new ANNidx[1]; ANNdistArray dists = new ANNdist[1];
				kdTree->annkSearch(ptSearched, 1, nnIdx, dists);
				int nnIndexPt = nnIdx[0];
				double nnDist = dists[0];
				if (nnDist < MIN_DIST)
				{
					CPoint nnVertex = secondMesh->get_vertex(nnIndexPt);
					CPoint nnNormal = secondMesh->get_normal(nnIndexPt);
					CPoint newVertex = (nnVertex + firstVertIter) / 2.0;
					CPoint newNormal = (nnNormal + firstNormIter) / 2.0;
					mergedVertexList.push_back(newVertex);
					mergedNormalList.push_back(newNormal);
					secondVertexOverlapped[nnIndexPt] = true;
				}
				else
				{
					mergedVertexList.push_back(firstVertIter);
					mergedNormalList.push_back(firstNormIter);
				}
			}

			for (size_t sv = 0; sv < secondVertexList.size(); sv++)
			{
				if (!secondVertexOverlapped[sv])
				{
					mergedVertexList.push_back(secondVertexList[sv]);
					mergedNormalList.push_back(secondNormalList[sv]);
				}
			}

			mergedMesh = new PlyCloud(mergedVertexList, mergedNormalList);
			drawMergedMeshOK();
		}
		else
		{
			std::vector<CPoint> mergedVertexList;
			std::vector<bool> secondVertexOverlapped;
			secondVertexOverlapped.assign(secondVertexList.size(), false);

			for (size_t v = 0; v < firstVertexList.size(); v++)
			{
				CPoint firstVertIter = firstVertexList[v];
				ANNpoint ptSearched = annAllocPt(3);
				ptSearched[0] = firstVertIter[0];
				ptSearched[1] = firstVertIter[1];
				ptSearched[2] = firstVertIter[2];
				ANNidxArray nnIdx = new ANNidx[1]; ANNdistArray dists = new ANNdist[1];
				kdTree->annkSearch(ptSearched, 1, nnIdx, dists);
				int nnIndexPt = nnIdx[0];
				double nnDist = dists[0];
				if (nnDist < MIN_DIST)
				{
					CPoint nnVertex = secondMesh->get_vertex(nnIndexPt);
					//CPoint nnNormal = secondMesh->get_normal(nnIndexPt);
					CPoint newVertex = (nnVertex + firstVertIter) / 2.0;
					mergedVertexList.push_back(newVertex);
					secondVertexOverlapped[nnIndexPt] = true;
				}
				else
				{
					mergedVertexList.push_back(firstVertIter);
				}
			}

			for (size_t sv = 0; sv < secondVertexList.size(); sv++)
			{
				if (!secondVertexOverlapped[sv])
				{
					mergedVertexList.push_back(secondVertexList[sv]);
				}
			}

			mergedMesh = new PlyCloud(mergedVertexList);
			drawMergedMeshOK();
		}

		isMeshesMerged = true;
	}
}

void MultipleMeshViewer::viewSplitMeshes()
{
	showMergedMesh = false;
	updateGL();
}

void MultipleMeshViewer::drawMergedMeshOK()
{
	showMergedMesh = true;
	updateGL();
}

void MultipleMeshViewer::drawMergedMesh()
{
	std::cout << "MultipleMeshViewer::drawMergedMesh" << std::endl;

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

	std::vector<CPoint> vertexList = mergedMesh->get_vertex_list();
	std::vector<CPoint> normalList = mergedMesh->get_normal_list();
	size_t normalSize = normalList.size();
	size_t vertexSize = vertexList.size();
	std::cout << vertexSize << " " << normalSize << std::endl;
	bool normalExist = false;
	if (normalSize == vertexSize)
	{
		normalExist = true;
	}
	glm::vec3 color = colorList[0];
	for (size_t j = 0; j < vertexList.size(); j++)
	{
		CPoint vert = vertexList[j];
		
		glPointSize(10);
		glColor3d(color[0], color[1], color[2]);
		glBegin(GL_POINTS);
		glVertex3d(vert[0], vert[1], vert[2]);
		if (normalExist)
		{
			CPoint norl = normalList[j];
			glNormal3d(norl[0], norl[1], norl[2]);
		}
		glEnd();
	}

}