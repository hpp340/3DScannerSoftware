#include "showmesh.h"

showMesh::showMesh(QWidget *parent) :
    QGLWidget(parent)
{
    initGL();
    loadPointCloud("W:\\Codes\\2014SummerMSC\\3DScannerSoftware\\mainWindow\\genus2ply.ply");
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

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    std::cout << "widget wh " << this->width() << " " << this->height() << std::endl;
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

void showMesh::paintGL()
{
    double horizontalAngle1 = 0.0;
    // double horizontalAngle2 = 0.0;
    double verticalAngle1 = 0.0;
    // double verticalAngle2 = 0.0;
    double viewDist = 2.0;

    std::cout << "23333" << std::endl;

    // to calculate the up vector used for gluLookAt
    glm::vec3 viewPoint = glm::vec3(1.0 * sin(horizontalAngle1), 1.0 * sin(verticalAngle1), 1.0*cos(horizontalAngle1));
    glm::vec3 rightVec = glm::vec3(-cos(horizontalAngle1), 0.0, 1.0*sin(horizontalAngle1));
    glm::vec3 upVector = glm::cross(-viewPoint, rightVec);

    gluLookAt(viewDist*sin(horizontalAngle1), viewDist*sin(verticalAngle1), viewDist*cos(horizontalAngle1), 0.0, 0.0, 0.0, upVector.x, upVector.y, upVector.z);
    glPushMatrix();

    // draw point cloud
    // drawPointCloud(pointCloud1);

    std::vector<CPoint> vertexList = pointCloud->get_vertex_list();
    std::vector<CPoint> normalList = pointCloud->get_normal_list();
    for (size_t i = 0; i < vertexList.size(); i++)
    {
        CPoint vertex = vertexList[i];
        CPoint normal = normalList[i];
        // std::cout << vertex[0] << vertex[1] << vertex[2] << std::endl;
        glPointSize(2.5);
        glBegin(GL_POINTS);
        glColor3d(1.0, 1.0, 1.0);
        glVertex3d(vertex[0], vertex[1], vertex[2]);
        glNormal3d(normal[0], normal[1], normal[2]);
        glEnd();
    }
    glPopMatrix();

    //glFlush();

    // glutSwapBuffers();
}
