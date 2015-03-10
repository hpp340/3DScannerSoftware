#pragma once



#include <QWidget>
#include "SensorViewer.h"
#include <QVboxLayout>
#include <OpenNI.h>
#include <QToolbar> 
#include <QAction>
#include <QMessageBox>
#include <QStatusBar>
#include "header\PlyCloud.h"
#include <QLabel>
#include <QSignalMapper>

/*! \brief sensorWindow creates a window instance to show the scanned point cloud by scanner.
*  \details The sensor is started once the window is created. And it contains buttons to start scanning and stop scanning.
*/
class sensorWindow :
	public QWidget
{
	Q_OBJECT
public:
	/*! Constructor. set maximum depth and start sensor.
	*/
	sensorWindow(int _maxDepth);
	~sensorWindow();

	/*! Return whether the automatic registration from RGB to depth image is turned on or not.
	*/
	bool isRgbToDepthColorConverter() { return rgbToDepthCoordConverter; }

private:
	/*! Initialize the sensor window. Create buttons and toolbar.
	*/
	void initSensorWindow();
	/*! Start the sensor so that it can scan.
	*/
	openni::Status startSensor();
	// reconstruct point cloud from depth stream
	//void reconPointCloud(openni::VideoStream &, openni::VideoStream &);

private slots:

	void updateStatusBarToScan();
	void updateStatusBarToView();
	void updateStatusBarToAlign();
	void debug();

private:

	/*! sensorViewer is the major class to show the scanned point cloud.
	* If the start scanning button is clicked, it is reponsible for recording the point cloud and writting to disk.
	*/
	SensorViewer * sensorViewer;

	/*! scannedMesh is used to store the point cloud. Once it's reconstructed, it can be rendered or written to disk.
	*/
	PlyCloud * scannedMesh;

	/*! Toolbar in this window
	*/
	QToolBar * sensorTB;

	/*! Video depth stream. Get from API of OPENNI2. All depth information is acquried from these streams.
	*/
	openni::VideoStream depthStream;

	/*! Video RGB stream. Get from API of OPENNI2. All RGB information is acquried from these streams.
	*/
	openni::VideoStream rgbStream;

	/*! Scanner Device. In our setting, it is ID of PrimeSense.
	*/
	openni::Device device;

	/*! maxDepthRange is specified in the constructor. Objects out of this range will not be rendered nor recorded.
	*/
	int maxDepthRange;

	/*! If the registration from RGB to depth is turned, this is true, which will produce better alignment between RGB and depth.
	*/
	bool rgbToDepthCoordConverter;

	/*! Single depth frame from the video stream.
	*/
	openni::VideoFrameRef depthFrame;

	/*! Single RGB frame from the video stream.
	*/
	openni::VideoFrameRef rgbFrame;

	/*! Button for start scanning
	*/
	QAction * startScan;
	
	/*! Button for stop scanning
	*/
	QAction * stopScan;

	/*! Button for ICP Reconstruction
	*/
	QAction * icpRecon;

	QStatusBar * statusBar;

	QLabel * labelInStatusBar;

};

