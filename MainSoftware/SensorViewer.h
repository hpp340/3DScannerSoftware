/* Used to view the scanned scene from PrimeSense Sensor
* Since we don't need the axis to view the scene
* Author: Jerome Jiang
* Date: 9/11/2014
*/

#pragma once
#include "meshviewer.h"
#include "OpenGLHeader.h"
#include <OpenNI.h>

class SensorViewer :
	public MeshViewer
{
	Q_OBJECT
public:

	SensorViewer(openni::VideoStream &depth, openni::VideoStream &color, bool rgbToDepthRegConverter);
	~SensorViewer();

	void run();

private:
	void drawMeshColorPoints();

protected:
	// overwrite because we don't need axis in this scene
	void drawAxis();
	// overwrite
	void paintGL();
	// overwrite
	// only draw points - with or without color
	void drawMesh();

public:
	void initSensorViewer();

protected:
	openni::VideoStream & m_depthStream;
	openni::VideoStream & m_rgbStream;
	openni::VideoStream** m_streams;

	openni::VideoFrameRef m_depthFrame;
	openni::VideoFrameRef m_rgbFrame;

	bool m_rgbToDepthRegConverter;

private:
	int videoWidth;
	int videoHeight;

private slots:
	void updateDisplay();
};

