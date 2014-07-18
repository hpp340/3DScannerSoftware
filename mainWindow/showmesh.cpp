#include "showmesh.h"

showMesh::showMesh(QWidget *parent) :
    QWidget(parent)
{
}

showMesh::~showMesh()
{
}

// initialize the GL setting
void showMesh::initGL()
{
    initDisplay();
    initLight();
}

void showMesh::initDisplay()
{
    //initializeGLFunctions();
    //qglClearColor(Qt::black);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0, this->width(), this->height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(fovy, (GLdouble)this->width()/this->height(), zNear, zFar);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void showMesh::initLight()
{
    GLfloat lightPos[] = { 0.0, 0.0, 1.0, 1.0 };
    GLfloat lightColor[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat ambient[] = { 0.1f, 0.1f, 0.1f, 1.0f };
    glShadeModel(GL_SMOOTH);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightColor);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
}

void showMesh::loadPointCloud(const char * filename)
{
    pointCloud = new PlyCloud;
    pointCloud->read_ply(filename);
}

void showMesh::displayMesh()
{
    std::vector<CPoint> vertexList = pointCloud->get_vertex_list();
    std::vector<CPoint> normalList = pointCloud->get_normal_list();
    for (size_t i = 0; i < vertexList.size(); i++)
    {
        CPoint vertex = vertexList[i];
        CPoint normal = normalList[i];
        glPointSize(2.5);
        glBegin(GL_POINTS);
        glColor3d(1.0, 1.0, 1.0);
        glVertex3d(vertex[0], vertex[1], vertex[2]);
        glNormal3d(normal[0], normal[1], normal[2]);
        glEnd();
    }
}
