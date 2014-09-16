#include "SensorViewer.h"
#include <QTimer>

SensorViewer::SensorViewer(openni::VideoStream &depth, openni::VideoStream &color) :
m_depthStream(depth), m_rgbStream(color), m_streams(NULL)
{
	videoWidth = videoHeight = 0;
}

SensorViewer::~SensorViewer()
{
}

void SensorViewer::initSensorViewer()
{
	std::cout << "SensorViewer:initSensorViewer..." << std::endl;
	openni::VideoMode depthMode, rgbMode;

	if (m_depthStream.isValid() && m_rgbStream.isValid())
	{
		std::cout << "both valid" << std::endl;
		depthMode = m_depthStream.getVideoMode();
		std::cout << "both valid 1" << std::endl;

		rgbMode = m_rgbStream.getVideoMode();
		std::cout << "both valid 2" << std::endl;

		int depthWidth = depthMode.getResolutionX();
		std::cout << "both valid 3" << std::endl;

		int depthHeight = depthMode.getResolutionY();
		std::cout << "both valid 4" << std::endl;

		int rgbWidth = rgbMode.getResolutionX();
		std::cout << "both valid 5" << std::endl;

		int rgbHeight = rgbMode.getResolutionY();
		std::cout << "both valid 233" << std::endl;

		if ((depthWidth == rgbWidth) && (depthHeight == rgbHeight))
		{
			videoWidth = depthWidth;
			videoHeight = depthHeight;
		}
		else
		{
			std::cout << "sensorWindow: Error: Rgb and depth stream expected to be the same resolution." << std::endl;
			std::cout << "depthWidth " << depthWidth << " depthHeight " << depthHeight << std::endl;
			std::cout << "rgbWidth " << rgbWidth << " rgbHeight " << rgbHeight << std::endl;

			//return openni::STATUS_ERROR;
		}
	}
	else if (m_depthStream.isValid())
	{
		depthMode = m_depthStream.getVideoMode();
		int depthWidth = depthMode.getResolutionX();
		int depthHeight = depthMode.getResolutionY();
		videoWidth = depthWidth;
		videoHeight = depthHeight;
	}
	else if (m_rgbStream.isValid())
	{
		rgbMode = m_rgbStream.getVideoMode();
		int rgbWidth = rgbMode.getResolutionX();
		int rgbHeight = rgbMode.getResolutionY();
		videoWidth = rgbWidth;
		videoHeight = rgbHeight;
	}
	else
	{
		std::cout << "sensorWindow: Error: No Valid Video stream..." << std::endl;
		//return openni::STATUS_ERROR;
	}
	std::cout << "SensorViewer:initSensorViewer...almost finished" << std::endl;

	m_streams = new openni::VideoStream*[2];
	m_streams[0] = &m_depthStream;
	m_streams[1] = &m_rgbStream;

	std::cout << "SensorViewer:initSensorViewer...finished" << std::endl;

}

// overwrite
void SensorViewer::drawAxis()
{
	return;
}

void SensorViewer::run()
{
	QTimer * timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
	timer->start(50);
	//updateGL();
}

void SensorViewer::updateDisplay()
{
	updateGL();
}

void SensorViewer::paintGL()
{
	int changedIdx;
	std::cout << "SensorViewer: paintGL..." << std::endl;
	//openni::Status rc = openni::OpenNI::waitForAnyStream(m_streams, 2, &changedIdx);
	//std::cout << "3333333" << std::endl;

	//if (rc != openni::STATUS_OK)
	//{
	//	printf("Wait failed\n");
	//	return;
	//}

	//switch (changedIdx)
	//{
	//case 0:
	//	m_depthStream.readFrame(&m_depthFrame); break;
	//case 1:
	//	m_rgbStream.readFrame(&m_rgbFrame); break;
	//default:
	//	printf("Error in wait\n");
	//}

	openni::Status rc = m_depthStream.readFrame(&m_depthFrame);
	const openni::DepthPixel * depthCoorArray = NULL;
	//m_rgbStream.readFrame(&rgbFrame);
	if (rc == openni::STATUS_OK)
	{
		depthCoorArray = static_cast<const openni::DepthPixel*>(m_depthFrame.getData());
	}

	//int numPixels = videoWidth * videoHeight;
	std::vector<CPoint> vertexList;

	if (depthCoorArray != NULL)
	{
		for (int y = 0; y < m_depthFrame.getHeight(); ++y)
		{
			for (int x = 0; x < m_depthFrame.getWidth(); ++x)
			{
				int idx = x + y * m_depthFrame.getWidth();
				const openni::DepthPixel & zValue = depthCoorArray[idx];
				if (zValue != 0)
				{
					float fx, fy, fz;
					openni::CoordinateConverter::convertDepthToWorld(m_depthStream, x, y, zValue, &fx, &fy, &fz);
					CPoint newVertex((double)fx, (double)fy, (double)fz);
					vertexList.push_back(newVertex);
				}
			}
		}
	}

	//for (size_t i = 0; i < vertexList.size(); i++)
	//{
	//	CPoint v = vertexList[i];
	//	std::cout << v[0] << " " << v[1] << " " << v[2] << std::endl;
	//}

	pointCloud = new PlyCloud(vertexList);
	setDefaultDrawMode();
	std::cout << "SensorViewer: start drawing..." << std::endl;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	drawMesh();
	glPopMatrix();
	//sensorViewer->acceptMesh(scannedMesh);
	//getchar();
	//return openni::STATUS_OK;

}