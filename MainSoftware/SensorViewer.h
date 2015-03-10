#pragma once

#include "meshviewer.h"
#include "OpenGLHeader.h"
#include "SensorScanner.h"
#include "ScanWriteThread.h"
#include <OpenNI.h>
#include <QTimer>
#include <fstream>
#include <QThread>
#include <QPainter>


enum class SENSOR_MODE {SENSOR_VIEW, SENSOR_SCAN, SENSOR_ALIGNMENT};

/*!
* \brief Used to view the scanned scene from PrimeSense Sensor.
* \details SensorViewer is a inherited class of MeshViewer. It is mainly reponsible for rendering the scanned point cloud realtime. At the same time, if the start scan button is clicked, the class will do the job of writting the point cloud onto hard disk.
* \details Since the computation of point cloud is finished at the same time the data is collected, the scan speed could be 200ms per frame. According to the different data amount, the scan speed could float between 150ms to 300ms.
* \details While the scan is finished, a timerecord.txt will be written and it records timestamp for each frame.
*/
class SensorViewer :
	public MeshViewer
{
	Q_OBJECT

typedef unsigned short ushort;

public:

	/*! Constructor. All paramenters are passed from SensorWindow
	* \param depth reference to the depth video stream
	* \param color reference to the color video stream
	* \param rgbToDepthRegConverter the flag indicating whether the registration from RGB to depth is turned on or not
	* \param _maxDepthRange maximum depth range for scanning
	*/
	SensorViewer(openni::VideoStream &depth, openni::VideoStream &color, bool rgbToDepthRegConverter, int _maxDepthRange);

	/*! Destructor */
	~SensorViewer();

	/*! start the rendering process to show what the scanner sees. Rendering point cloud.
	*/
	void run();

private:

	/*! Draw the point cloud
	*/
	void drawMeshColorPoints();

protected:

	/*! overwrite because we don't need axis in this scene
	*/
	void drawAxis();

	/*! Qt function which should be overwrite.
	*/
	void paintGL();

	/*! overwrite
	* only draw points - with or without color.
	*/
	void drawMesh();

public:

	/*! Initialize the sensor viewer with OpenGL stuff. This should be called right after the constructor.
	*/
	void initSensorViewer();

protected:

	/*! depth video stream. Passed in from SensorWindow.
	*/
	openni::VideoStream & m_depthStream;

	/*! RGB video stream. Passed in from SensorWindow.
	*/
	openni::VideoStream & m_rgbStream;

	/*! Array containing both video streams.
	*/
	openni::VideoStream** m_streams;

	/*! depth frame. Parsed from depth stream m_depthStream
	*/
	openni::VideoFrameRef m_depthFrame;

	/*! RGB frame. Parsed from depth stream m_depthStream
	*/
	openni::VideoFrameRef m_rgbFrame;

	/*! flag indicating whether the registration from RGB to depth is turned on or not.
	*/
	bool m_rgbToDepthRegConverter;

private:

	/*! Width of the video stream
	*/
	int videoWidth;

	/*! Height of the video stream
	*/
	int videoHeight;

	/*! Maximum depth range out of which the objects will not be scanned
	*/
	int maxDepthRange;

	//int minDepthRange;

	/*! Number of files that recorded and written on disk.*/
	int numFile;

	/*! Timer for scanning. Now this is used for rendering. 250ms per frame.*/
	QTimer * scanTimer;

	/*! flag indicating if the scanning process is stopped. */
	bool isScanStopped;

	/*! flag indicating if the scanning process is started. */
	bool hasScanStarted;

	//debug need
	//std::ofstream debugOutput;

	/*! record the time of each recorded point cloud frame in millisecond */
	std::ofstream timerecord;

	SensorScanner * scanThread;
	//QThread * scanThread;

	std::ofstream renderTime;

	/// Scanned Sequence
	std::vector<PlyCloud *> scannedSequence;

	/// Thread used to write all the scanned frames onto disk
	ScanWriteThread * writeThread;

	/// Thread used to scan @ 10fps
	QThread * newScanThread;

	/// An enum class used to indicate the current mode of sensor
	SENSOR_MODE sensorMode;

private slots:
	
	/*! Slot that used to re-render the whole scene. Every 250ms this is triggered.*/
	void updateDisplay();
	//void dataCollectionOneFrame();

public slots:

	/*! Slot used to start to record the point clouds */
	void viewerStartScan();

	/*! Slot used to stop recording the point clouds */
	void viewerStopScan();

	/*! Slot used to start ICP to align the recorded point cloud */
	void startICP();
};

