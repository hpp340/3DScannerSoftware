#include "meshviewer.h"

MeshViewer::MeshViewer(QWidget *_parent) : QGLWidget(_parent)
{
	pointCloud = new PlyCloud();
	init();
}

MeshViewer::~MeshViewer()
{
}

void MeshViewer::init()
{
	// qt init
	setAttribute(Qt::WA_NoSystemBackground, true);
	setFocusPolicy(Qt::StrongFocus);

	center = glm::vec3(0.0f, 0.0f, 0.0f);
	radius = 0.0;
	horizontalAngle1 = 0.0;
	horizontalAngle2 = 0.0;
	verticalAngle1 = 0.0;
	verticalAngle2 = 0.0;
	viewDist = 2.0;
}

QSize MeshViewer::sizeHint() const
{
	QRect rectangle = QApplication::desktop()->screenGeometry();
	return QSize(int(rectangle.width()*0.96), int(rectangle.height()));
}

void MeshViewer::setScene(glm::vec3 scenePosCenter, GLdouble sceneRadius)
{
	center = scenePosCenter;
	radius = sceneRadius;

	updateProjectionMatrix();
}

void MeshViewer::updateProjectionMatrix()
{
	makeCurrent();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	std::cout << "width " << width() << " height " << height() << std::endl;
	gluPerspective(fovy(), (GLdouble)width() / (GLdouble)height(), 0.01 * radius, 100.0 * radius);
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glMatrixMode(GL_MODELVIEW);

	// update scene model view matrix

	GLdouble translation0 = matModelView[0] * center[0] + matModelView[4] * center[1] + matModelView[8] * center[2] + matModelView[12];
	GLdouble translation1 = matModelView[1] * center[0] + matModelView[5] * center[1] + matModelView[9] * center[2] + matModelView[13];
	GLdouble translation2 = matModelView[2] * center[0] + matModelView[6] * center[1] + matModelView[10] * center[2] + matModelView[14] + 3.0*radius;
	glm::vec3 transVector = glm::vec3(-translation0, -translation1, -translation2);
	// change matrix and translation
	makeCurrent();
	glLoadIdentity();
	glTranslated(transVector[0], transVector[1], transVector[2]);
	glMultMatrixd(matModelView);
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
}

void MeshViewer::loadFile(const char * meshfile)
{
	pointCloud->read_ply(meshfile);
}

void MeshViewer::initializeGL()
{
	initDisplay();
	initLights();

	// get the projection and modelview matrix
	glGetDoublev(GL_PROJECTION_MATRIX, matProjection);
	glGetDoublev(GL_MODELVIEW_MATRIX, matModelView);
	glGetIntegerv(GL_VIEWPORT, viewPort);

	// set scene center and size of view. 1.0 for radius
	setScene(center, 1.0);
}

void MeshViewer::initDisplay()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, width(), height());
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy(), (double)width() / height(), zNear(), zFar());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
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
}

void MeshViewer::paintGL()
{
	glm::vec3 viewPoint = glm::vec3(1.0 * sin(horizontalAngle2), 1.0 * sin(verticalAngle2), 1.0*cos(horizontalAngle2));
	glm::vec3 rightVec = glm::vec3(-cos(horizontalAngle2), 0.0, 1.0*sin(horizontalAngle2));
	glm::vec3 upVector = glm::cross(-viewPoint, rightVec);
	std::cout << "233" << std::endl;
	// to calculate the up vector used for gluLookAt
	// gluLookAt(viewDist*sin(horizontalAngle2), viewDist*sin(verticalAngle2), viewDist*cos(horizontalAngle2), 0.0, 0.0, 0.0, upVector.x, upVector.y, upVector.z);

	glPushMatrix();
	// draw mesh(point cloud)
	drawMesh();
	glPopMatrix();
	//glFlush();
}

void MeshViewer::drawMesh()
{
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