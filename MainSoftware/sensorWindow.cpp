#include "sensorWindow.h"

sensorWindow::sensorWindow()
{
	std::cout << "sensorWindow:sensorWindow constructor..." << std::endl;
	sensorViewer = new MeshViewer();
	depthWidth = depthHeight = rgbWidth = rgbHeight = videoWidth = videoHeight = 0;
	initSensorWindow();
	startSensor();
	//reconPointCloud();
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

void sensorWindow::startSensor()
{
	std::cout << "sensorWindow: startSensor..." << std::endl;
	openni::Status rc = openni::STATUS_OK;
	openni::Device device;
	openni::VideoStream depthStream;
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
		//return openni::STATUS_ERROR;
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
		}
		std::cout << "sensorWindow: depth stream start successfully" << std::endl;
	}
	else
	{
		std::cout << "sensorWindow: Can't find depth stream " << openni::OpenNI::getExtendedError() << std::endl;
	}

	//// start rgb stream
	//rc = rgbStream.create(device, openni::SENSOR_COLOR);
	//if (rc == openni::STATUS_OK)
	//{
	//	rc = rgbStream.start();
	//	if (rc != openni::STATUS_OK)
	//	{
	//		std::cout << "sensorWindow: Error: rgb sensor start failed." << std::endl;
	//		rgbStream.destroy();
	//	}
	//	std::cout << "sensorWindow:rgb stream start successfully" << std::endl;
	//}
	//else
	//{
	//	std::cout << "sensorWindow: Can't find rgb stream " << openni::OpenNI::getExtendedError() << std::endl;
	//}

	//if (!depthStream.isValid() || !rgbStream.isValid())
	//{
	//	std::cout << "sensorWindow: No Valid Stream. Exiting..." << std::endl;
	//	openni::OpenNI::shutdown();
	//	//return openni::STATUS_ERROR;
	//}
	//std::cout << "sensorWindow: method startSensor() execs successfully." << std::endl;
	//// reconstruct point cloud

	////return openni::STATUS_OK;

	//openni::Status rc = openni::STATUS_OK;

	//openni::Device device;
	//openni::VideoStream depth, color;
	//const char* deviceURI = openni::ANY_DEVICE;

	//rc = openni::OpenNI::initialize();

	//printf("After initialization:\n%s\n", openni::OpenNI::getExtendedError());

	//rc = device.open(deviceURI);
	//if (rc != openni::STATUS_OK)
	//{
	//	printf("SimpleViewer: Device open failed:\n%s\n", openni::OpenNI::getExtendedError());
	//	openni::OpenNI::shutdown();
	//	//return 1;
	//}

	//rc = depth.create(device, openni::SENSOR_DEPTH);
	//if (rc == openni::STATUS_OK)
	//{
	//	rc = depth.start();
	//	if (rc != openni::STATUS_OK)
	//	{
	//		printf("SimpleViewer: Couldn't start depth stream:\n%s\n", openni::OpenNI::getExtendedError());
	//		depth.destroy();
	//	}
	//}
	//else
	//{
	//	printf("SimpleViewer: Couldn't find depth stream:\n%s\n", openni::OpenNI::getExtendedError());
	//}

	//rc = color.create(device, openni::SENSOR_COLOR);
	//if (rc == openni::STATUS_OK)
	//{
	//	rc = color.start();
	//	if (rc != openni::STATUS_OK)
	//	{
	//		printf("SimpleViewer: Couldn't start color stream:\n%s\n", openni::OpenNI::getExtendedError());
	//		color.destroy();
	//	}
	//}
	//else
	//{
	//	printf("SimpleViewer: Couldn't find color stream:\n%s\n", openni::OpenNI::getExtendedError());
	//}

	//if (!depth.isValid() || !color.isValid())
	//{
	//	printf("SimpleViewer: No valid streams. Exiting\n");
	//	openni::OpenNI::shutdown();
	//	//return 2;
	//}
}

//void sensorWindow::reconPointCloud()
//{
//	std::cout << "sensorWindow: reconPointCloud" << std::endl;
//	openni::VideoMode depthMode, rgbMode;
//
//	if (depthStream.isValid() && rgbStream.isValid())
//	{
//		depthMode = depthStream.getVideoMode();
//		rgbMode = rgbStream.getVideoMode();
//
//		depthWidth = depthMode.getResolutionX();
//		depthHeight = depthMode.getResolutionY();
//		rgbWidth = rgbMode.getResolutionX();
//		rgbHeight = rgbMode.getResolutionY();
//
//		if ((depthWidth == rgbWidth) && (depthHeight == rgbHeight))
//		{
//			videoWidth = depthWidth;
//			videoHeight = videoHeight;
//		}
//		else
//		{
//			std::cout << "sensorWindow: Error: Rgb and depth stream expected to be the same resolution." << std::endl;
//			std::cout << "depthWidth " << depthWidth << " depthHeight " << depthHeight << std::endl;
//			std::cout << "rgbWidth " << rgbWidth << " rgbHeight " << rgbHeight << std::endl;
//
//			//return openni::STATUS_ERROR;
//		}
//	}
//	else if (depthStream.isValid())
//	{
//		depthMode = depthStream.getVideoMode();
//		depthWidth = depthMode.getResolutionX();
//		depthHeight = depthMode.getResolutionY();
//		videoWidth = depthWidth;
//		videoHeight = videoHeight;
//	}
//	else if (rgbStream.isValid())
//	{
//		rgbMode = rgbStream.getVideoMode();
//		rgbWidth = rgbMode.getResolutionX();
//		rgbHeight = rgbMode.getResolutionY();
//		videoWidth = rgbWidth;
//		videoHeight = rgbHeight;
//	}
//	else
//	{
//		std::cout << "sensorWindow: Error: No Valid Video stream..." << std::endl;
//		//return openni::STATUS_ERROR;
//	}
//
//	openni::Status rc = depthStream.readFrame(&depthFrame);
//	const openni::DepthPixel * depthCoorArray = NULL;
//	rgbStream.readFrame(&rgbFrame);
//	if (rc == openni::STATUS_OK)
//	{
//		depthCoorArray = static_cast<const openni::DepthPixel*>(depthFrame.getData());
//	}
//
//	//int numPixels = videoWidth * videoHeight;
//	std::vector<CPoint> vertexList;
//
//	if (depthCoorArray != NULL)
//	{
//		for (int y = 0; y < depthFrame.getHeight(); ++y)
//		{
//			for (int x = 0; x < depthFrame.getWidth(); ++x)
//			{
//				int idx = x + y * depthFrame.getWidth();
//				const openni::DepthPixel & zValue = depthCoorArray[idx];
//				if (zValue != 0)
//				{
//					float fx, fy, fz;
//					openni::CoordinateConverter::convertDepthToWorld(depthStream, x, y, zValue, &fx, &fy, &fz);
//					CPoint newVertex((double)fx, (double)fy, (double)fz);
//					vertexList.push_back(newVertex);
//				}
//			}
//		}
//	}
//
//	scannedMesh = new PlyCloud(vertexList);
//	//sensorViewer->acceptMesh(scannedMesh);
//	//getchar();
//	//return openni::STATUS_OK;
//}
