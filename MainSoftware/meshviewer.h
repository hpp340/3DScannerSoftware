#ifndef MESHVIEWER_H
#define MESHVIEWER_H

#include <vector>
#include <QGLWidget>
#include <QApplication>
#include <QDesktopWidget>
#include "OpenGLHeader.h"
#include "header\PlyCloud.h"
#include "header\glm\glm.hpp"
#include "header\glm\gtx\normal.hpp"

class MeshViewer : public QGLWidget
{
    Q_OBJECT
public:
    // constructor
    MeshViewer(QWidget *_parent = 0);
    // deconstructor
    ~MeshViewer();
	// load mesh file
	void loadFile(const char * );

	// return scene center
	glm::vec3 getCenter() { return center; }

	// return radius
	GLdouble getRadius() { return radius; }

	// size hint
	QSize sizeHint() const;

private:
	// initialize the members
	void init();
	// initialize GL - triggered by Qt
	void initializeGL();
	// draw scene - triggered by Qt
	void paintGL();
	// resize - triggered by Qt
	void resizeGL();

	// set scene position and radius(size) 
	// center is used as the fixed point for rotation and adjustion for camera&view
	void setScene(glm::vec3, GLdouble);

protected:
	// initialize display functions
	void initDisplay();
	// initialize light functioins - we may need 3 lights
	void initLights();
	// return fovy
	float fovy() { return 45.0f; }
	// return zNear
	float zNear() { return 1.0f; }
	// return zFar
	float zFar() { return 5.0f; }

	// method for draw mesh
	void drawMesh();

	// update projection matrix and make the whole scene visiable on the scene
	// make camera/eye far enough 
	void updateProjectionMatrix();

private:
	// point cloud
	PlyCloud * pointCloud;
	// scene center
	glm::vec3 center;
	// scene radius
	GLdouble radius;

	// view angles and distances
	GLdouble horizontalAngle1;
	GLdouble horizontalAngle2;
	GLdouble verticalAngle1;
	GLdouble verticalAngle2;
	GLdouble viewDist;

	// projection matrix and modelview matrix
	GLdouble matModelView[16], matProjection[16];
	GLint viewPort[4];
};

#endif // MESHVIEWER_H
