
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
#include "header\SOIL\SOIL.h"

#ifndef PI
#define PI 3.14159265
#endif

/*! \brief Draw mode used in MeshViewer. 
* - POINTS: only point cloud
* - WIREFRAME: wireframe mode. only edges
* - FLATLINES: flat shading with edges
* - SMOOTH: smooth shading without edges
*/
enum class DRAW_MODE { NONE, POINTS, WIREFRAME, FLATLINES, FLAT, SMOOTH };

/*!
* \brief Base class for all the viewers in this software.
* \details Use OpenGL to render point cloud or triangle meshes, with simple iteractive functions.
*/
class MeshViewer : public QGLWidget
{
	Q_OBJECT
public:

	/*! Constructor 
	*/
	MeshViewer(QWidget *_parent = 0);

	/*! Deconstructor
	*/
	~MeshViewer();

	/// Load mesh file 
	/// - sExt is the file extension
	virtual void loadFile(const char *, string sExt);

	/*! Save mesh file 
	* - sExt is the file extension
	*/
	virtual void saveFile(const char *, string sExt);

	/*! Return scene center.
	* Used to put the whole mesh or point cloud in the center of the window.
	*/
	glm::vec3 getCenter() { return center; }

	/*! Return radius */
	GLdouble getRadius() { return radius; }

	/*! Return trackball radius
	* Trackball is used to rotate and zoom in the scene
	*/
	double getTrackballRadius() { return trackballRadius; }

	/*! size hint. Used by Qt Lib */
	QSize sizeHint() const;

	/*! Get point cloud shown in current viewer */
	PlyCloud * getMesh() { return pointCloud; };

	/*! Return the filename of current file */
	std::string getFilename();

	/*! Accept mesh from other classes. This method is only used to render the mesh after Poisson Surface Reconstruction. */
	void acceptMesh(PlyCloud *);

	/*! Set draw mode (Render mode). Point Cloud, wireframe, flat shading or smooth shading. */
	void setDrawMode(DRAW_MODE);

protected:

	/// Initialize the members
	void init();

	/// Initialize OpenGL - triggered by Qt
	void initializeGL();

	/// Draw scene - triggered by Qt. This shoule be overwritten to meet our needs.
	virtual void paintGL();

	/// Resize - triggered by Qt. Once the window is resized, this method is called automatically.
	void resizeGL(int width, int height);

	/// Set scene position and radius(size) 
	/// center is used as the fixed point for rotation and adjustion for camera&view
	void setScene(glm::vec3, GLdouble);

	/// Do the rotation specified by axis and angle
	void rotate(glm::vec3, double);

	/// Do the translation specified by the transVector
	void translate(glm::vec3 transVector);

	/// Get bounding box of the mesh
	void getBoundingBox();

protected:

	/// Initialize display functions
	void initDisplay();

	/// Initialize light functioins - we may need 3 lights
	void initLights();

	/// Initialize material
	void initMaterial();

	/// Return fovy
	float fovy() const { return 45.0f; }

	/// Return zNear
	float zNear() { return 0.01 * radius; }

	/// Return zFar
	float zFar() { return 100 * radius; }

	/// Initialize texture
	bool initTexture();

	/// Return modelview matrix
	GLdouble * getModelViewMatrix() { return matModelView; }

	/// Return projection matrix
	GLdouble * getProjectionMatrix() { return matProjection; }

	/// Return viewport matrix
	GLint * getViewPort() { return viewPort; }

	/// Method for drawing mesh
	virtual void drawMesh();

	/// Method for drawing axises
	virtual void drawAxis();

	/// Draw point
	virtual void drawMeshPoints();

	/// Draw colorful points
	void MeshViewer::drawMeshColorPoints();

	/// Draw wireframe
	virtual void drawMeshWireframe();

	/// Draw flat lines
	virtual void drawMeshFlatlines();

	/// Draw flat
	virtual void drawMeshFlat();

	/// Draw smooth
	virtual void drawMeshSmooth();

	/// Set default draw mode
	void setDefaultDrawMode();

	/// Update projection matrix  
	void updateProjectionMatrix();

	/// Make the whole scene visiable on the scene
	/// Make camera/eye far enough
	void makeWholeSceneVisible();

	/// QT mouse event
	/// Use virtual method in case we would need to inheritate the class in the future
	/// override qt pre-defined method - cant be changed to other names
	virtual void mousePressEvent(QMouseEvent *);
	virtual void mouseReleaseEvent(QMouseEvent *);
	virtual void mouseMoveEvent(QMouseEvent *);
	virtual void wheelEvent(QWheelEvent *);
	//virtual void keyPressEvent(QKeyEvent *);

	/// Rotation and translation 
	/// Rotate and translate the view, instead of the point cloud itself
	void rotationView(QPoint newMousePos);
	void translateView(QPoint newMousePos);

	/// Arcball algorithm, used to rotation & translation
	/// map a 2D screen position to a unit sphere
	bool arcball(QPoint screenPos, glm::vec3 &new3DPos);

protected:
	/// Trackball radius, used for controll rotation
	double trackballRadius;

protected:
	/// Point cloud rendered in current viewer
	PlyCloud * pointCloud;

	/// Check if mesh is loaded
	bool isMeshLoaded;

	/// flag indicating if light is on
	bool isLightOn;

	/// flag indicating if texture is on
	bool isTextureOn;

	/// flag indicating if texture is loaded
	bool isTextureLoaded;

	/// scene center
	glm::vec3 center;

	/// scene radius
	GLdouble radius;

	/// texture ID
	GLuint textID;

	/// projection matrix and modelview matrix
	GLdouble matModelView[16], matProjection[16];
	GLint viewPort[4];

	/// mouse related variable
	int mouseButton; /// indicating which mouse button

	/// indicating latest mouse position
	QPoint latestMousePos; 

	/// indicating latest mouse 3D position - transformed from 2D position
	glm::vec3 latestMouse3DPos;	

	/// indicating if the latest mouse position could be mapped onto the arc ball
	bool isLatestMouseOK; 

	/// boundingbox
	glm::vec3 minBoundingBox;

	/// boundingbox
	glm::vec3 maxBoundingBox;
	
	/// draw mode - point cloud, wireframe, flat shading or smooth shading
	DRAW_MODE meshDrawMode;

protected:

	/// filename of the openned mesh in this viewer
	QString filename;

	/// filename of the openned mesh in this viewer, in C++ string
	std::string sFilename;

	/// filename extension. used to check if the file is illegal
	std::string sTexFilename;

	// slots
public slots:

	/// Slots used to open the mesh file
	void openMesh();

	/// Slots used to save the mesh to file
	void saveMesh();


	/// Slot used for light control - turn the light on
	void turnOnLight();

	/// Slot used for light control - turn the light off
	void turnOffLight();

	/// Slot used for texture control - turn the texture on
	void textureOn();

	/// Slot used for texture control - turn the texture off
	void textureOff();

signals:

	/// Signal omitted if the DRAW_MODE is set to WIREFRAME
	void setDrawModeWireframe();

	/// Signal omitted if the DRAW_MODE is set to POINTS
	void setDrawModePoints();
};

#endif // MESHVIEWER_H
