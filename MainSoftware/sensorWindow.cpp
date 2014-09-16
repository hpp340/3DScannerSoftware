#include "sensorWindow.h"

sensorWindow::sensorWindow()
{
	std::cout << "sensorWindow:sensorWindow constructor..." << std::endl;
	//sensorViewer = new SensorViewer();
	depthWidth = depthHeight = rgbWidth = rgbHeight = videoWidth = videoHeight = 0;
	startSensor();
}


sensorWindow::~sensorWindow()
{
}

void sensorWindow::initSensorWindow()
{
	this->setGeometry(300, 300, 1800, 1350);

	QVBoxLayout * vLayout = new QVBoxLayout;
	sensorTB = new QToolBar();
	vLayout->addWidget(sensorTB);
	vLayout->addWidget(sensorViewer);
	this->setLayout(vLayout);
	std::cout << "sensorWindow: sensorWindow initialize..." << std::endl;
}

openni::Status sensorWindow::startSensor()
{
	std::cout << "sensorWindow: startSensor..." << std::endl;
	openni::Status rc = openni::STATUS_OK;
	// video stream must be defined as local variables
	// bugs will be reported if they are defined as class members

	const char* deviceURI = openni::ANY_DEVICE;

	rc = openni::OpenNI::initialize();
	std::cout << "sensorWindow:Initialization...if Error: " << openni::OpenNI::getExtendedError() << std::endl;
	
	// turn on sensor
	rc = device.open(deviceURI);
	if (rc != openni::STATUS_OK)
	{
		std::cout << "sensorWindow:Device cannot be turned on..." << std::endl;
		QMessageBox turnOnFail;
		turnOnFail.setText("sensorWindow:Device cannot be turned on...");
		turnOnFail.exec();
		// turn off the sensor if fail to turn it on
		openni::OpenNI::shutdown();
		return openni::STATUS_ERROR;
	}
	std::cout << "sensorWindow: sensor turned on successfully..." << std::endl;

	// start depth stream
	rc = depthStream.create(device, openni::SENSOR_DEPTH);
	if (rc == openni::STATUS_OK)
	{
		rc = depthStream.start();
		if (rc != openni::STATUS_OK)
		{
			std::cout << "sensorWindow: Error: depth sensor start failed." << std::endl;
			QMessageBox depthFail;
			depthFail.setText("sensorWindow: Error: depth sensor start failed.");
			depthFail.exec();
			depthStream.destroy();
			return rc;
		}
		std::cout << "sensorWindow: depth stream start successfully" << std::endl;
	}
	else
	{
		std::cout << "sensorWindow: Can't find depth stream " << openni::OpenNI::getExtendedError() << std::endl;
		return rc;
	}

	// start rgb stream
	rc = rgbStream.create(device, openni::SENSOR_COLOR);
	if (rc == openni::STATUS_OK)
	{
		rc = rgbStream.start();
		if (rc != openni::STATUS_OK)
		{
			std::cout << "sensorWindow: Error: rgb sensor start failed." << std::endl;
			QMessageBox rgbFail;
			rgbFail.setText("sensorWindow: Error: rgb sensor start failed.");
			rgbFail.exec();
			rgbStream.destroy();
			return rc;
		}
		std::cout << "sensorWindow:rgb stream start successfully" << std::endl;
	}
	else
	{
		std::cout << "sensorWindow: Can't find rgb stream " << openni::OpenNI::getExtendedError() << std::endl;
		return rc;
	}

	if (!depthStream.isValid() || !rgbStream.isValid())
	{
		std::cout << "sensorWindow: No Valid Stream. Exiting..." << std::endl;
		openni::OpenNI::shutdown();
		return openni::STATUS_ERROR;
	}
	std::cout << "sensorWindow: method startSensor() execs successfully." << std::endl;
	//reconPointCloud(depthStream, rgbStream);
	sensorViewer = new SensorViewer(depthStream, rgbStream);
	initSensorWindow();
	sensorViewer->initSensorViewer();
	sensorViewer->run();

	return openni::STATUS_OK;
}

//void sensorWindow::reconPointCloud(openni::VideoStream &m_depthStream, openni::VideoStream &m_rgbStream)
//{
//	std::cout << "sensorWindow: reconPointCloud" << std::endl;
//	//openni::VideoMode depthMode, rgbMode;
//
//	//if (m_depthStream.isValid() && m_rgbStream.isValid())
//	//{
//	//	depthMode = m_depthStream.getVideoMode();
//	//	rgbMode = m_rgbStream.getVideoMode();
//
//	//	depthWidth = depthMode.getResolutionX();
//	//	depthHeight = depthMode.getResolutionY();
//	//	rgbWidth = rgbMode.getResolutionX();
//	//	rgbHeight = rgbMode.getResolutionY();
//
//	//	if ((depthWidth == rgbWidth) && (depthHeight == rgbHeight))
//	//	{
//	//		videoWidth = depthWidth;
//	//		videoHeight = videoHeight;
//	//	}
//	//	else
//	//	{
//	//		std::cout << "sensorWindow: Error: Rgb and depth stream expected to be the same resolution." << std::endl;
//	//		std::cout << "depthWidth " << depthWidth << " depthHeight " << depthHeight << std::endl;
//	//		std::cout << "rgbWidth " << rgbWidth << " rgbHeight " << rgbHeight << std::endl;
//
//	//		//return openni::STATUS_ERROR;
//	//	}
//	//}
//	//else if (m_depthStream.isValid())
//	//{
//	//	depthMode = m_depthStream.getVideoMode();
//	//	depthWidth = depthMode.getResolutionX();
//	//	depthHeight = depthMode.getResolutionY();
//	//	videoWidth = depthWidth;
//	//	videoHeight = videoHeight;
//	//}
//	//else if (m_rgbStream.isValid())
//	//{
//	//	rgbMode = m_rgbStream.getVideoMode();
//	//	rgbWidth = rgbMode.getResolutionX();
//	//	rgbHeight = rgbMode.getResolutionY();
//	//	videoWidth = rgbWidth;
//	//	videoHeight = rgbHeight;
//	//}
//	//else
//	//{
//	//	std::cout << "sensorWindow: Error: No Valid Video stream..." << std::endl;
//	//	//return openni::STATUS_ERROR;
//	//}
//
//	//openni::VideoStream ** streams;
//	//streams = new openni::VideoStream*[2];
//	//streams[0] = &m_depthStream;
//	//streams[1] = &m_rgbStream;
//
//	//int changedIndex;
//	//openni::Status rc = openni::OpenNI::waitForAnyStream(streams, 2, &changedIndex);
//
//	//if (rc != openni::STATUS_OK)
//	//{
//	//	printf("Wait failed\n");
//	//	return;
//	//}
//
//	//switch (changedIndex)
//	//{
//	//case 0:
//	//	m_depthStream.readFrame(&depthFrame); break;
//	//case 1:
//	//	m_rgbStream.readFrame(&rgbFrame); break;
//	//default:
//	//	printf("Error in wait\n");
//	//}
//
//	////openni::Status rc = m_depthStream.readFrame(&depthFrame);
//	//const openni::DepthPixel * depthCoorArray = NULL;
//	////m_rgbStream.readFrame(&rgbFrame);
//	//if (rc == openni::STATUS_OK)
//	//{
//	//	depthCoorArray = static_cast<const openni::DepthPixel*>(depthFrame.getData());
//	//}
//
//	////int numPixels = videoWidth * videoHeight;
//	//std::vector<CPoint> vertexList;
//
//	//if (depthCoorArray != NULL)
//	//{
//	//	for (int y = 0; y < depthFrame.getHeight(); ++y)
//	//	{
//	//		for (int x = 0; x < depthFrame.getWidth(); ++x)
//	//		{
//	//			int idx = x + y * depthFrame.getWidth();
//	//			const openni::DepthPixel & zValue = depthCoorArray[idx];
//	//			if (zValue != 0)
//	//			{
//	//				float fx, fy, fz;
//	//				openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
//	//				CPoint newVertex((double)fx, (double)fy, (double)fz);
//	//				vertexList.push_back(newVertex);
//	//			}
//	//		}
//	//	}
//	//}
//
//	////for (size_t i = 0; i < vertexList.size(); i++)
//	////{
//	////	CPoint v = vertexList[i];
//	////	std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl;
//	////}
//
//	//scannedMesh = new PlyCloud(vertexList);
//	//sensorViewer->acceptMesh(scannedMesh);
//	//getchar();
//	//return openni::STATUS_OK;
//}
