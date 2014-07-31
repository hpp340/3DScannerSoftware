#ifndef MESHVIEWER_H
#define MESHVIEWER_H

#include <vector>
#include <QGLWidget>
#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include "OpenGLHeader.h"
#include "header\PlyCloud.h"
#include "header\glm\glm.hpp"
#include "header\glm\gtx\norm.hpp"

#include "JFace.h"

#ifndef PI
#define PI 3.14159265
#endif

enum class DRAW_MODE {NONE, POINTS, WIREFRAME, FLATLINES, FLAT, SMOOTH};

class MeshViewer : public QGLWidget
{
    Q_OBJECT
public:
    // constructor
    MeshViewer(QWidget *_parent = 0);
    // deconstructor
    ~MeshViewer();
	// load mesh file
	void loadFile(const char *);
	// save mesh file
	virtual void saveFile(const char *);
	// return scene center
	glm::vec3 getCenter() { return center; }

	// return radius
	GLdouble getRadius() { return radius; }
	// return trackball radius
	double getTrackballRadius() { return trackballRadius; }

	// size hint
	QSize sizeHint() const;

	// get point cloud
	PlyCloud * getMesh() { return pointCloud; };
	// return the filename
	std::string getFilename();
	// accept mesh from other classes
	void acceptMesh(PlyCloud *);
	// set draw mode
	void setDrawMode(DRAW_MODE);

private:
	// initialize the members
	void init();
	// initialize GL - triggered by Qt
	void initializeGL();
	// draw scene - triggered by Qt
	virtual void paintGL();
	// resize - triggered by Qt
	void resizeGL(int width, int height);

	// set scene position and radius(size) 
	// center is used as the fixed point for rotation and adjustion for camera&view
	void setScene(glm::vec3, GLdouble);

	// do the rotation specified by axis and angle
	void rotate(glm::vec3, double);
	// do the translation specified by the transVector
	void translate(glm::vec3 transVector);

	// get bounding box of the mesh
	void getBoundingBox();

protected:
	// initialize display functions
	void initDisplay();
	// initialize light functioins - we may need 3 lights
	void initLights();
	// initialize material
	void initMaterial();
	// return fovy
	float fovy() const { return 45.0f; }
	// return zNear
	float zNear() { return 0.01 * radius; }
	// return zFar
	float zFar() { return 100 * radius; }
	// return modelview matrix
	GLdouble * getModelViewMatrix() { return matModelView; }
	// return projection matrix
	GLdouble * getProjectionMatrix() { return matProjection; }
	// return viewport matrix
	GLint * getViewPort() { return viewPort; }

	// method for drawing mesh
	virtual void drawMesh();
	// method for drawing axises
	void drawAxis();
	// draw point
	void drawMeshPoints();
	// draw wireframe
	void drawMeshWireframe();
	// draw flat lines
	void drawMeshFlatlines();
	// draw flat
	void drawMeshFlat();
	// draw smooth
	void drawMeshSmooth();
	// set default draw mode
	void setDefaultDrawMode();
	// update projection matrix  
	void updateProjectionMatrix();

	// make the whole scene visiable on the scene
	// make camera/eye far enough
	void makeWholeSceneVisible();

	// QT mouse event
	// use virtual method in case we would need to inheritate the class in the future
	// override qt pre-defined method - cant be changed to other names
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void wheelEvent(QWheelEvent *);
	//virtual void keyPressEvent(QKeyEvent *);

	// rotation and translation 
	// rotate and translate the view, instead of the point cloud itself
	void rotationView(QPoint newMousePos);
	void translateView(QPoint newMousePos);

	// arcball algorithm, used to rotation & translation
	// map a 2D screen position to a unit sphere
	bool arcball(QPoint screenPos, glm::vec3 &new3DPos);

protected:
	// trackball radius, used for controll rotation
	double trackballRadius;

protected:
	// point cloud
	PlyCloud * pointCloud;
	// is mesh loaded
	bool isMeshLoaded;
	// is light on
	bool isLightOn;
	// scene center
	glm::vec3 center;
	// scene radius
	GLdouble radius;

	// projection matrix and modelview matrix
	GLdouble matModelView[16], matProjection[16];
	GLint viewPort[4];

	// mouse related variable
	int mouseButton; // which mouse button
	QPoint latestMousePos; // latest mouse position
	glm::vec3 latestMouse3DPos;	// latest mouse 3D position
	bool isLatestMouseOK; // if the latest mouse position could be mapped onto the arc ball
	// boundingbox
	glm::vec3 minBoundingBox;
	glm::vec3 maxBoundingBox;
	// draw mode
	DRAW_MODE meshDrawMode;

private:
	QString filename;
	std::string sFilename;

	// slots
public slots:
	void openMesh();
	void saveMesh();
	// light control
	void turnOnLight();
	void turnOffLight();

signals:
	void setDrawModeWireframe();
	void setDrawModePoints();
};

#endif // MESHVIEWER_H
