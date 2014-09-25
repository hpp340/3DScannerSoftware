#include "sensorWindow.h"

sensorWindow::sensorWindow()
{
	std::cout << "sensorWindow:sensorWindow constructor..." << std::endl;
	rgbToDepthCoordConverter = false;
	startSensor();
}

sensorWindow::~sensorWindow()
{
}

void sensorWindow::initSensorWindow()
{
	this->setGeometry(300, 300, 1800, 1350);

	QVBoxLayout * vLayout = new QVBoxLayout;

	startScan = new QAction(tr("Start Scanning"), this);
	startScan->setIcon(QIcon(":/icons/images/startscan.png"));
	startScan->setText(tr("Start Scanning..."));
	startScan->setStatusTip(tr("Start Scanning such that a complete model is reconstructed"));
	connect(startScan, SIGNAL(triggerred()), sensorViewer, SLOT(startScan()));

	stopScan = new QAction(tr("Stop Scanning"), this);
	stopScan->setIcon(QIcon(":/icons/images/stopscan.png"));
	stopScan->setText(tr("Stop Scanning..."));
	stopScan->setStatusTip(tr("Stop Scanning to start the global registration"));
	connect(stopScan, SIGNAL(triggerred()), sensorViewer, SLOT(stopScan()));

	sensorTB = new QToolBar();
	sensorTB->addAction(startScan);
	sensorTB->addAction(stopScan);

	vLayout->addWidget(sensorTB);
	vLayout->addWidget(sensorViewer);
	this->setLayout(vLayout);
	std::cout << "sensorWindow: sensorWindow initialize..." << std::endl;
}

openni::Status sensorWindow::startSensor()
{
	std::cout << "sensorWindow: startSensor..." << std::endl;
	// openni::Status rc = openni::STATUS_OK;
	// video stream must be defined as local variables
	// bugs will be reported if they are defined as class members

	const char* deviceURI = openni::ANY_DEVICE;
	
	if (openni::OpenNI::initialize() != openni::STATUS_OK)
	{
		std::cout << "SensorWindow: Initialization error: " << openni::OpenNI::getExtendedError() << std::endl;
		return openni::STATUS_ERROR;
	}

	// turn on sensor
	if (device.open(deviceURI) != openni::STATUS_OK)
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
	if (device.hasSensor(openni::SENSOR_DEPTH))
	{
		if (depthStream.create(device, openni::SENSOR_DEPTH) == openni::STATUS_OK)
		{
			if (depthStream.start() == openni::STATUS_OK)
			{
				std::cout << "sensorWindow: depth stream start successfully" << std::endl;
			}
			else
			{
				std::cout << "sensorWindow: Error: depth sensor start failed." << std::endl;
				QMessageBox depthFail;
				depthFail.setText("sensorWindow: Error: depth sensor start failed.");
				depthFail.exec();
				depthStream.destroy();
				return openni::STATUS_ERROR;
			}
		}
		else
		{
			std::cout << "sensorWindow: depth stream can't be created." << std::endl;
			return openni::STATUS_ERROR;
		}
	}
	else
	{
		std::cout << "SensorWindow: there is no depth sensor..." << std::endl;
		return openni::STATUS_ERROR;
	}

	// start rgb stream
	if (device.hasSensor(openni::SENSOR_COLOR))
	{
		if (rgbStream.create(device, openni::SENSOR_COLOR) == openni::STATUS_OK)
		{
			if (device.isImageRegistrationModeSupported(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR))
			{
				device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
			}
			else
			{
				std::cout << "sensorWindow: This device doesn't support IMAGE_REGISTRATION_DEPTH_TO_COLOR. Will use Coordinate Converter." << std::endl;
				rgbToDepthCoordConverter = true;
			}
			if (rgbStream.start() == openni::STATUS_OK)
			{
				std::cout << "sensorWindow: rgb stream start successfully" << std::endl;
			}
			else
			{
				std::cout << "sensorWindow: Error: rgb sensor start failed." << std::endl;
				QMessageBox rgbFail;
				rgbFail.setText("sensorWindow: Error: rgb sensor start failed.");
				rgbFail.exec();
				rgbStream.destroy();
				return openni::STATUS_ERROR;
			}
		}
		else
		{
			std::cout << "sensorWindow: rgb stream can't be created." << std::endl;
			return openni::STATUS_ERROR;
		}
	}
	else
	{
		std::cout << "SensorWindow: there is no rgb sensor..." << std::endl;
		return openni::STATUS_ERROR;
	}

	if (!depthStream.isValid() || !rgbStream.isValid())
	{
		std::cout << "sensorWindow: No Valid Stream. Exiting..." << std::endl;
		openni::OpenNI::shutdown();
		return openni::STATUS_ERROR;
	}

	std::cout << "sensorWindow: method startSensor() execs successfully." << std::endl;

	sensorViewer = new SensorViewer(depthStream, rgbStream, rgbToDepthCoordConverter);
	initSensorWindow();
	sensorViewer->initSensorViewer();
	sensorViewer->run();

	return openni::STATUS_OK;
}
