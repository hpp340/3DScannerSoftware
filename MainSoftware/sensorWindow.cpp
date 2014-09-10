#include "sensorWindow.h"

sensorWindow::sensorWindow()
{
	sensorViewer = new MeshViewer();
	initSensorWindow();
	startSensor();
}


sensorWindow::~sensorWindow()
{
}

void sensorWindow::initSensorWindow()
{
	this->setGeometry(300, 300, 1800, 1350);

	QVBoxLayout * vLayout = new QVBoxLayout;
	vLayout->addWidget(sensorViewer);
	this->setLayout(vLayout);
}

void sensorWindow::startSensor()
{
	openni::Status rc = openni::STATUS_OK;
	openni::Device device;
	openni::VideoStream depthStream, rgbStream;
	const char* deviceURI = openni::ANY_DEVICE;

	rc = openni::OpenNI::initialize();
	std::cout << "if Error: " << openni::OpenNI::getExtendedError() << std::endl;
	
	rc = device.open(deviceURI);
	if (rc != openni::STATUS_OK)
	{
		std::cout << "sensorWindow:Device cannot be turned on..." << std::endl;
		return;
	}

	rc = depthStream.create(device, openni::SENSOR_DEPTH);

}
