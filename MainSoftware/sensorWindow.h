#pragma once
#include <QWidget>
#include "SensorViewer.h"
#include <QVboxLayout>
#include <OpenNI.h>
#include <QToolbar> 
#include <QAction>
#include <QMessageBox>
#include "header\PlyCloud.h"

class sensorWindow :
	public QWidget
{
	Q_OBJECT
public:
	sensorWindow(int _maxDepth);
	~sensorWindow();

	bool isRgbToDepthColorConverter() { return rgbToDepthCoordConverter; }

private:
	void initSensorWindow();
	openni::Status startSensor();
	// reconstruct point cloud from depth stream
	//void reconPointCloud(openni::VideoStream &, openni::VideoStream &);

private:
	SensorViewer * sensorViewer;
	// reconstructed point cloud from depth stream
	PlyCloud * scannedMesh;
	QToolBar * sensorTB;
	openni::VideoStream depthStream, rgbStream;
	openni::Device device;
	int maxDepthRange;
	bool rgbToDepthCoordConverter;
	openni::VideoFrameRef depthFrame, rgbFrame;

	QAction * startScan;
	QAction * stopScan;
	QAction * icpRecon;
};

