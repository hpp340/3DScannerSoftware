#pragma once
#include <QWidget>
#include "SensorViewer.h"
#include <QVboxLayout>
#include <OpenNI.h>
#include <QToolbar>
#include <QMessageBox>
#include "header\PlyCloud.h"

class sensorWindow :
	public QWidget
{
	Q_OBJECT
public:
	sensorWindow();
	~sensorWindow();

private:
	void initSensorWindow();
	openni::Status startSensor();
	// reconstruct point cloud from depth stream
	void reconPointCloud(openni::VideoStream &, openni::VideoStream &);

private:
	SensorViewer * sensorViewer;
	// reconstructed point cloud from depth stream
	PlyCloud * scannedMesh;
	QToolBar * sensorTB;
	int depthWidth, depthHeight, rgbWidth, rgbHeight;
	int videoWidth, videoHeight;

	openni::VideoFrameRef depthFrame, rgbFrame;
};

