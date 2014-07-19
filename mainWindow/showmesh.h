#ifndef SHOWMESH_H
#define SHOWMESH_H

#include <Windows.h>

#include "headers/PlyCloud.h"
#include "headers/Point.h"

#include <QGLWidget>
#include <gl/GL.h>
#include <gl/GLU.h>

#include "headers/glm/glm.hpp"
#include "headers/glm/gtx/norm.hpp"

using namespace MeshLib;

class showMesh : public QGLWidget
{
    Q_OBJECT
public:
    explicit showMesh(QWidget *parent = 0);
    ~showMesh();
    void loadPointCloud(const char *);

protected:
    void initGL(void);
    void paintGL(void);

private:
    void initDisplay(void);
    void initLight(void);
signals:

public slots:


private:
    double fovy = 80.0;
    double zNear = 1.0;
    double zFar = 5.0;
    PlyCloud * pointCloud;
};

#endif // SHOWMESH_H
