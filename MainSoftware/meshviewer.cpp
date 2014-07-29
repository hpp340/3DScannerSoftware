#include "meshviewer.h"

MeshViewer::MeshViewer(QWidget *_parent) : QGLWidget(_parent)
{
	pointCloud = new PlyCloud();
	init();
	std::cout << "constructor" << std::endl;
}

MeshViewer::~MeshViewer()
{
	std::cout << "deconstructor" << std::endl;
}

void MeshViewer::init()
{
	// qt init
	setAttribute(Qt::WA_NoSystemBackground, true);
	setFocusPolicy(Qt::StrongFocus);

	center = glm::vec3(0.0f, 0.0f, 0.0f);
	radius = 0.0;
	trackballRadius = 0.6;
	isMeshLoaded = false;
	isLightOn = true;
	std::cout << "init" << std::endl;
}

QSize MeshViewer::sizeHint() const
{
	QRect rectangle = QApplication::desktop()->screenGeometry();
	return QSize(int(rectangle.width()*0.96), int(rectangle.height()));
	system("PAUSE");
	std::cout << "sizeHint" << std::endl;
}

void MeshViewer::resizeGL(int width, int height)
{
	std::cout << "resizing...." << std::endl;
	std::cout << "width " << width << "height " << height << std::endl;
	glViewport(0, 0, width, height);
	glGetIntegerv(GL_VIEWPORT, viewPort);
	updateProjectionMatrix();
	updateGL();
}

void MeshViewer::getBoundingBox()
{
	GLdouble xMin, yMin, zMin;
	xMin = yMin = zMin = 99999.99;
	GLdouble xMax, yMax, zMax;
	xMax = yMax = zMax = -99999.99;

	std::vector<CPoint> vertexList = pointCloud->get_vertex_list();
	for (size_t i = 0; i < vertexList.size(); i++)
	{
		CPoint _vertex = vertexList[i];
		if (_vertex[0] < xMin) { xMin = _vertex[0]; }
		if (_vertex[0] > xMax) { xMax = _vertex[0]; }
		if (_vertex[1] < yMin) { yMin = _vertex[1]; }
		if (_vertex[1] > yMax) { yMax = _vertex[1]; }
		if (_vertex[2] < zMin) { zMin = _vertex[2]; }
		if (_vertex[2] > zMax) { zMax = _vertex[2]; }
	}
	minBoundingBox[0] = xMin; minBoundingBox[1] = yMin; minBoundingBox[2] = zMin;
	maxBoundingBox[0] = xMax; maxBoundingBox[1] = yMax; maxBoundingBox[2] = zMax;
}

void MeshViewer::loadFile(const char * meshfile)
{
	bool isLoadOK = pointCloud->read_ply(meshfile);
	if (! isLoadOK)
	{
		QMessageBox loadFail;
		loadFail.setText("Can't Open File.");
		loadFail.exec();
	}
	// get bounding box of the mesh
	getBoundingBox();
	isMeshLoaded = true;
	std::cout << "loadFile" << std::endl;
}

void MeshViewer::acceptMesh(PlyCloud * outerMesh)
{
	std::cout << "acceptMesh" << std::endl;
	free(pointCloud);
	pointCloud = outerMesh;
	getBoundingBox();
	isMeshLoaded = true;
	updateGL();
}

void MeshViewer::saveFile(const char * meshfile)
{
	bool isWriteOK = pointCloud->write_ply(meshfile);
	if (! isWriteOK)
	{
		QMessageBox writeFail;
		writeFail.setText("Can't Save File.");
		writeFail.exec();
	}
	std::cout << "saveFile" << std::endl;
}

void MeshViewer::initializeGL()
{
	// initialize display and lights
	initDisplay();
	initMaterial();
	initLights();

	// get the projection and modelview matrix
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetIntegerv(GL_VIEWPORT, viewPort);

	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_COLOR_MATERIAL);
	// set scene center and size of view. 1.0 for radius
	setScene(center, 1.0);
	std::cout << "initializeGL" << std::endl;
}

void MeshViewer::initDisplay()
{
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy(), (double)width() / height(), zNear(), zFar());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	std::cout << "initDisplay" << std::endl;
}

void MeshViewer::initMaterial()
{
	// material
	GLfloat matA[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat matD[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat matS[] = { 0.4f, 0.4f, 0.4f, 1.0f };
	GLfloat shine[] = { 120.0f };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, matA);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, matD);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, matS);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shine);
}

void MeshViewer::initLights()
{
	// first light - light 0
	GLfloat lightPos0[] = { 0.1f, 0.1f, -0.02f, 0.0f };
	GLfloat lightColor0[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat ambient0[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient0);

	// second light - light 1
	GLfloat lightPos1[] = { -0.1f, 0.1f, -0.02f, 0.0f };
	GLfloat lightColor1[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat ambient1[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, lightColor1);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient1);

	// third light - light 2
	GLfloat lightPos2[] = { 0.0f, 0.0f, 0.1f, 0.0f };
	GLfloat lightColor2[] = { 0.7f, 0.7f, 0.7f, 1.0f };
	GLfloat ambient2[] = { 0.1f, 0.1f, 0.1f, 1.0f };
	glShadeModel(GL_SMOOTH);
	glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, lightColor2);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient2);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	std::cout << "initLight" << std::endl;
}

void MeshViewer::setScene(glm::vec3 scenePosCenter, GLdouble sceneRadius)
{
	center = scenePosCenter;
	radius = sceneRadius;

	updateProjectionMatrix();
	makeWholeSceneVisible();

	std::cout << "setScene" << std::endl;
}

void MeshViewer::updateProjectionMatrix()
{
	makeCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	std::cout << "width " << width() << " height " << height() << std::endl;
	gluPerspective(fovy(), (GLdouble)width() / (GLdouble)height(), 0.01 * radius, 100.0 * radius);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	std::cout << "updateProjectionMatrix" << std::endl;
}

void MeshViewer::makeWholeSceneVisible()
{
	// update scene model view matrix
	GLdouble translation0 = matModelView[0] * center[0] + matModelView[4] * center[1] + matModelView[8] * center[2] + matModelView[12];
	GLdouble translation1 = matModelView[1] * center[0] + matModelView[5] * center[1] + matModelView[9] * center[2] + matModelView[13];
	GLdouble translation2 = matModelView[2] * center[0] + matModelView[6] * center[1] + matModelView[10] * center[2] + matModelView[14] + 3.0*radius;
	glm::vec3 transVector = glm::vec3(-translation0, -translation1, -translation2);
	// change matrix and translation
	makeCurrent();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslated(transVector[0], transVector[1], transVector[2]);
	glMultMatrixd(matModelView);
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);

	std::cout << "makeWholeSceneVisible" << std::endl;
}

void MeshViewer::paintGL()
{
	std::cout << "paintGL" << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	if (isMeshLoaded)
	{
		drawAxis();
	}
	drawMesh();
	glPopMatrix();
	//glFlush();
}

void MeshViewer::drawAxis()
{
	std::cout << "meshviewer:drawaxis" << std::endl;
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixd(matProjection);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(matModelView);
	QFont TEXTFONT("Courier", 12);
	double r = glm::l2Norm(maxBoundingBox - minBoundingBox) * 0.5f;
	glm::vec3 c = (maxBoundingBox + minBoundingBox) * 0.5f;
	glm::vec3 x = glm::vec3(1.0*r, 0, 0);
	glm::vec3 y = glm::vec3(0, 1.0*r, 0);
	glm::vec3 z = glm::vec3(0, 0, 1.0*r);
	glm::dvec3 temp;

	glDisable(GL_LIGHTING);
	glLineWidth(2.5);
	glColor3d(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(c[0], c[1], c[2]);
	temp = c + x;
	glVertex3d(temp[0], temp[1], temp[2]);
	glEnd();
	renderText(temp[0] + 0.01*r, temp[1], temp[2], "X", TEXTFONT);

	glColor3d(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
	glVertex3d(c[0], c[1], c[2]);
	temp = c + y;
	glVertex3d(temp[0], temp[1], temp[2]);
	glEnd();
	renderText(temp[0], temp[1] + 0.01*r, temp[2], "Y", TEXTFONT);

	glColor3d(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	glVertex3d(c[0], c[1], c[2]);
	temp = c + z;
	glVertex3d(temp[0], temp[1], temp[2]);
	glEnd();
	renderText(temp[0], temp[1], temp[2] + 0.01*r, "Z", TEXTFONT);
}

void MeshViewer::drawMesh()
{
	std::cout << "meshviewer:drawmesh start" << std::endl;
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

	std::vector<CPoint> vertexList = pointCloud->get_vertex_list();
	std::vector<CPoint> normalList = pointCloud->get_normal_list();
	std::cout << vertexList.size() << std::endl;
	if (normalList.size() == vertexList.size())
	{
		for (size_t i = 0; i < vertexList.size(); i++)
		{
			CPoint vert = vertexList[i];
			CPoint norl = normalList[i];

			glPointSize(10);
			glColor3d(0.1, 0.5, 0.8);
			glBegin(GL_POINTS);
			glVertex3d(vert[0], vert[1], vert[2]);
			glNormal3d(norl[0], norl[1], norl[2]);
			glEnd();
		}
	}
	else
	{
		for (size_t i = 0; i < vertexList.size(); i++)
		{
			CPoint vert = vertexList[i];

			glPointSize(10);
			glColor3d(0.1, 0.5, 0.8);
			glBegin(GL_POINTS);
			glVertex3d(vert[0], vert[1], vert[2]);
			glEnd();
		}
	}
	std::cout << "meshviewer:drawMesh" << std::endl;
}

void MeshViewer::mousePressEvent(QMouseEvent * mouseEvent)
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
}

void MeshViewer::mouseMoveEvent(QMouseEvent * mouseEvent)
{
	QPoint newMousePos = mouseEvent->pos();
	// enable OpenGL Context
	makeCurrent();
	if (isLatestMouseOK)
	{
		switch (mouseButton)
		{
		case Qt::LeftButton:
			// debug output
			std::cout << "Mouse Left Moving..." << std::endl;
			// rotate the view
			rotationView(newMousePos);
			break;
		case Qt::RightButton:
			std::cout << "Mouse Right Moving..." << std::endl;
			// translate the view
			translateView(newMousePos);
			break;
		default:
			break;
		}
	}

	latestMousePos = newMousePos;
	//std::cout << "starting debugging ..." << std::endl;
	isLatestMouseOK= arcball(latestMousePos, latestMouse3DPos);
	// update OpenGL, trigger re-draw
	updateGL();
}

void MeshViewer::mouseReleaseEvent(QMouseEvent * /*mouseEvent*/)
{
	mouseButton = Qt::NoButton;
	isLatestMouseOK = false;
	std::cout << "Mouse Release..." << std::endl;
}

void MeshViewer::wheelEvent(QWheelEvent * mouseEvent)
{
	// scroll the wheel to scale the view port
	double moveAmount = - (double)mouseEvent->delta() / (120.0*8.0);
	translate(glm::vec3(0.0, 0.0, moveAmount));
	updateGL();
	mouseEvent->accept();
}
//void MeshViewer::keyPressEvent(QKeyEvent * mouseEvent)
//{
//	// todo
//}

// rotate the view
void MeshViewer::rotationView(QPoint newPos)
{
	std::cout << "rotationView" << std::endl;
	glm::vec3 new3DPos;
	bool isNewPosHitArcball = arcball(newPos, new3DPos);
	if (isNewPosHitArcball)
	{
		// because we always rotate centered by the zero point
		// so the axis is simply the cross product of the new mouse 3D postion and the old mouse 3D position
		glm::vec3 rotationAxis = glm::cross(latestMouse3DPos, new3DPos);

		if (glm::l2Norm(rotationAxis) < 1e-7) // too small move
		{
			rotationAxis = glm::vec3(1.0, 0.0, 0.0);
		}
		else
		{
			// normalize
			rotationAxis = rotationAxis / glm::l2Norm(rotationAxis);
		}

		// compute the rotation angle
		// sin law
		glm::vec3 diff = latestMouse3DPos - new3DPos;
		double t = 0.5 * glm::l2Norm(diff) / trackballRadius;
		if (t < -1.0)
		{
			t = -1.0;
		}
		else if (t > 1.0)
		{
			t = 1.0;
		}
		// rotation angle
		double rotatioinAngle = 2.0 * asin(t) * 180 / PI;
		rotate(rotationAxis, rotatioinAngle);
	}
}

void MeshViewer::rotate(glm::vec3 axis, double angle)
{
	std::cout << "rotate" << std::endl;
	GLdouble translation0 = matModelView[0] * center[0] + matModelView[4] * center[1] + matModelView[8] * center[2] + matModelView[12];
	GLdouble translation1 = matModelView[1] * center[0] + matModelView[5] * center[1] + matModelView[9] * center[2] + matModelView[13];
	GLdouble translation2 = matModelView[2] * center[0] + matModelView[6] * center[1] + matModelView[10] * center[2] + matModelView[14];
	glm::vec3 transVector = glm::vec3(translation0, translation1, translation2);

	// modify the modelview matrix
	// first make it at the center of the screen
	makeCurrent();
	glLoadIdentity();
	glTranslated(transVector[0], transVector[1], transVector[2]);
	// then rotate it
	glRotated(angle, axis[0], axis[1], axis[2]);
	// move back to the center
	glTranslated(-transVector[0], -transVector[1], -transVector[2]);
	// update the matrix
	glMultMatrixd(matModelView);
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
}

// translate the view
void MeshViewer::translateView(QPoint newPos)
{
	double zVal = -(matModelView[2] * center[0] + matModelView[6] * center[1] + matModelView[10] * center[2] + matModelView[14]) /
		(matModelView[3] * center[0] + matModelView[7] * center[1] + matModelView[11] * center[2] + matModelView[15]);
	double screenAspect = width() / height();
	double top = tan(fovy() / 2.0f * PI / 180.0f) * zNear();
	double right = screenAspect * top;
	QPoint posDiff = latestMousePos - newPos;
	glm::vec3 transVector = glm::vec3(2.0*posDiff.x() / width() * right / zNear() * zVal, 
									-2.0*posDiff.y() / height() * top / zNear() * zVal,
									0.0f);

	translate(transVector);
	//2.0*dx / w*right / near_plane*z,
	//	-2.0*dy / h*top / near_plane*z,
}

void MeshViewer::translate(glm::vec3 transVector)
{
	makeCurrent();
	glLoadIdentity();
	glTranslated(-transVector[0], -transVector[1], -transVector[2]);
	glMultMatrixd(matModelView);
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
}

bool MeshViewer::arcball(QPoint screenPos, glm::vec3 &new3Dpos)
{
	std::cout << "arcball" << std::endl;
	double x = (2.0 * screenPos.x() - width()) / (double)width();
	double y = -(2.0 * screenPos.y() - height()) / (double)height();
	double norm = x * x + y * y;
	
	double trackballRSqr = trackballRadius * trackballRadius;
	glm::vec3 modelPostion;
	modelPostion[0] = x;
	modelPostion[1] = y;
	if (norm < 0.5 * trackballRSqr)
	{
		modelPostion[2] = sqrt(trackballRSqr - norm);
	}
	else
	{
		modelPostion[2] = 0.5 * trackballRSqr / sqrt(norm);
	}
	new3Dpos[0] = modelPostion[0];
	new3Dpos[1] = modelPostion[1];
	new3Dpos[2] = modelPostion[2];

	return true;
}

std::string MeshViewer::getFilename()
{
	if (!filename.isEmpty())
	{
		// convert QString to char *
		return sFilename;
	}
	else
	{
		std::cout << "No File" << std::endl;
		return NULL;
	}
}

// slots
void MeshViewer::openMesh()
{
	filename = QFileDialog::getOpenFileName(this,
		tr("Open mesh file"),
		tr("../models/"),
		tr("PLY Files (*.ply);;"
		"All Files (*)"));
	if (!filename.isEmpty())
	{
		// convert QString to char *
		QByteArray byteArray = filename.toUtf8();
		const char * _filename = byteArray.constData();
		printf("%s\n", _filename);
		sFilename = filename.toStdString();
		loadFile(_filename);
	}
}

void MeshViewer::saveMesh()
{
	QString saveFilename = QFileDialog::getSaveFileName(this,
		tr("Save Mesh File"),
		tr("../models/"),
		tr("PLY Files (*.ply);;"
		"All Files (*)"));
	if (! saveFilename.isEmpty())
	{
		// convert QString to char *
		QByteArray byteArray = saveFilename.toUtf8();
		const char * _saveFilename = byteArray.constData();
		saveFile(_saveFilename);
	}
}

void MeshViewer::turnOnLight()
{
	std::cout << "meshviewer:turn on light" << std::endl;
	isLightOn = true;
	updateGL();
}

void MeshViewer::turnOffLight()
{
	std::cout << "meshviewer:turn off light" << std::endl;
	isLightOn = false;
	updateGL();
}